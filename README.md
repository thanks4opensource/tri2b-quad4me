tri2b and quad4me: clockless arbitrated bit-level serial protocols
===================================================================

**tri2b** and **quad4me** are two clockless, bit-level serial communications protocols supporting arbitration between simultaneous data senders, without requiring any timing or CPU performance guarantees from their software implementations or hardware platforms


Contents  <a name="contents"></a>
--------
* [No Warranty](#no_warranty)
* [Description](#description)
    * [Features](#features)
    * [Drawbacks](#drawbacks)
    * [Hardware requirements](#hardware_requirements)
* [Motivation and Background](#motivation_and_background)
    * [Why not bit-bang I2C?](#why_not_bit_bang_i2c)
    * [What about CAN bus?](#what_about_CAN_bus)
    * [Why the silly names?](#why_the_silly_names)
* [The Protocols](#the_protocols)
    * [Two-level state machine](#two_level_state_machine)
        * [States](#states)
        * [Phases](#phases)
    * [tri2b protocol](#tri2b_protocol)
    * [quad4me protocol](#quad4me_protocol)
    * [Edge- vs level-based](#edge_vs_level_based)
    * [Arbitration phase](#arbitration_phase)
    * [(Almost) Timing-free](#almost_timing_free)
        * [One line held low at idle](#one_line_held_low_at_idle)
        * [Rise time](#rise_time)
    * [The failed promise of hardware swapover](#the_failed_promise_of_hardware_swapover)
    * [To interrupt or not to interrupt](#to_interrupt_or_not_to_interrupt)
    * [tri2b or quad4me -- which one?](#tri2b_or_quad4me_which_one)
* [The Example Implementations and Testbed](#the_example_implementations_and_testbed)
    * [Using/porting the code](#using_porting_the_code)
    * [The code itself](#the_code_itself)
        * [C coders](#c_coders)
        * [C++ coders](#c_plus_plus_coders)
        * [Assembly coders](#assembly_coders)
        * [Python coders](#python_coders)
        * [Other coders](#other_coders)
    * [The example build system](#the_example_build_system)
    * [Prerequisites/dependencies](#prerequisites_dependencies)
    * [Repository directories and files](#repository_directories_and_files)
        * [Base class methods implemented in derived classes](#derived_class_methods)
    * [The testbed](#the_testbed)
    * [Build variants](#build_variants)
        * [Protocol variants](#protocol_variants)
        * [Testbed variants](#testbed_variants)
        * [Debug variants](#debug_variants)
    * [GDB test environment](#gdb_test_environment)
    * [Example implementation enhancements/improvements](#enhancements_improvements)
* [Requests For Improvements, Information, and Enhancements (RFIIE)](#RFIIE)
    * [Generic Request](#RFIIE_generic_request)
    * [**Non-** Request](#RFIIE_non_request)
        * [Code Formatting](#RFIIE_code_formatting)
    * [Specific Requests](#specific_requests)



No Warranty  <a name="no_warranty"></a>
-----------
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the [GNU General Public License](LICENSE.txt)( along with this program.  If not, see <https://www.gnu.org/licenses/gpl.html>




Description <a name="description"></a>
-----------

#### Features <a name="features"></a>

**tri2b** and **quad4me** support the following capabilities:

* multiple *nodes* (see [below](#nodes_vs_masters)) on a single, multi-wire serial bus

* all nodes receive all messages sent by any (all) other nodes

* at bus idle, any number of nodes can simultaneously attempt to send a message

* arbitration to determine which node's simultaneously-sent message acquires the bus (to the exclusion of all others) controlled by sending node's priority

* optional: node priorities can be dynamic, changing over time via algorithm (e.g. least-recently-used)

* the protocols are clockless, no pre-determined bit-clock rate

* throughput is determined by node CPU and I/O speed, limited by open-drain hardware line rise time (see [Rise time](#rise_time))

* there are (almost) no requirements on CPU timing/speed -- all nodes wait for all others to respond to protocol state transitions (see [(Almost) timing-free](#almost_timing_free))


#### Drawbacks <a name="drawbacks"></a>

**tri2b** and **quad4me** have several significant drawbacks compared to other serial communications protocols:

* "bit-banged" in software; no hardware support (see [RFIIE: Hardware Support](#RFIIE_hardware_support) and [hardware swapover](#the_failed_promise_of_hardware_swapover)).

* require more hardware lines (3 for tri2c, 4 for quad4me) than other protocols (e.g. I2C). See [RFIIE: Fewer lines](#RFIIE_fewer_lines)

* one of the open-drain hardware lines must be held low when bus is idle (no communications taking place). See [RFIEE: No line low at idle](#RFIIE_no_line_low_at_idle)

* "feature" <a name="any_node_hangs"></a> of no timing requirements is also a drawback: any node can "hang" the bus/protocol indefinitely by not/slowly responding. Like I2C "clock stretching" but worse -- the protocols are "stretched" by default.

* tri2b requires reliable detection of rising edges on the hardware lines. See [tri2b or quad4me -- which one?](#tri2b_or_quad4me_which_one)

* did I mention the need for three (or four lines) instead of two? ;)


#### Hardware requirements <a name="hardware_requirements"></a>

* Three (tri2b) or four (quad4me) open-drain GPIO ports

* GPIO ports must support simultaneous writing to -- setting output low or high (high-Z) -- and reading the input state of attached line independently of the output/write setting.

* tri2b: <a name="edge_detection_requirement"></a>  Two of the GPIO ports must support hardware detection of falling edges on their communication lines. See [Edge- vs level-based](#edge_vs_level_based), below.

* Both tri2b and quad4me (if interrupt-driven, see [To interrupt or not to interrupt](#to_interrupt_or_not_to_interrupt), below): Support dynamic enabling/disabling interrupts on falling edges, independent and non-conflicting with the rising edge detection requirement of tri2b.




Motivation and Background <a name="motivation_and_background"></a>
--------------------------

(Material in this section is not necessary to understand the protocols or their implementations -- skip ahead to [The Protocols](#the_protocols). On the other hand, a quick skimming over wouldn't hurt.)

What? You're still here. OK, you asked for it ...

When I began designing the hardware project for which tri2b and quad4me were eventually developed, I quickly realized it required the simultaneous sending, arbitration, dynamic prioritization, and all-nodes-receive-all-messages capabilities which they now provide.

Those needs were all driven by the system's requirement for low latency <a name="latency"></a> above all else -- even data throughput, although throughput obviously figures into the total start-to-finish latency.

Being somewhat new to hardware development, I knew little about I2C and SPI other than their existence. My thought at the time was, "My requirements must be fairly standard. This has to be a solved problem in the industry. I'll just use whatever off-the-shelf technology fits best. That part of the project, at least, will be easy."

How wrong I was.

I quickly rejected SPI due to its requirement for individual select lines, one for each node, in addition to its clock and data lines. But reading about I2C I immediately came across, and was encouraged by:


> *NXP Semiconductors UM10204 I2C-bus specification and user manual Rev. 6, 4 April 2014*  ([1](#i2c_standard))

> 2\. I2C-bus features *(page 3 of UM10204)*

> * It is a true multi-master bus including collision detection and arbitration to prevent data corruption if two or more masters simultaneously initiate data transfer.

and:

> 3.1.8 Arbitration *(page 11 of UM10204)*

> Arbitration, like synchronization, refers to a portion of the protocol required only if more than one master is used in the system. Slaves are not involved in the arbitration procedure. A master may start a transfer only if the bus is free. Two masters may generate a START condition within the minimum hold time (t HD;STA ) of the START condition which results in a valid START condition on the bus. Arbitration is then required to determine which master will complete its transmission.

> Arbitration proceeds bit by bit. During every bit, while SCL is HIGH, each master checks to see if the SDA level matches what it has sent. This process may take many bits. Two masters can actually complete an entire transaction without error, as long as the transmissions are identical. The first time a master tries to send a HIGH, but detects that the SDA level is LOW, the master knows that it has lost the arbitration and turns off its SDA output driver. The other master goes on to complete its transaction.

> No information is lost during the arbitration process. A master that loses the arbitration can generate clock pulses until the end of the byte in which it loses the arbitration and must restart its transaction when the bus is free. If a master also incorporates a slave function and it loses arbitration during the addressing stage, it is possible that the winning master is trying to address it. The losing master must therefore switch over immediately to its slave mode.

<a name="i2c_standard"></a> *(1) It is my understanding that Philips developed I2C, and NXP acquired Philips, so I treat this document as somewhat of an official standard. Please correct me if I'm wrong.*

Sounded great. Just what I needed.

Unfortunately ...

After attempting to implement this on several MCUs for more weeks than I care to admit (let's not say "months", okay? no mention of "months") I was never able to get "I2C multi-master" mode to work reliably. I won't name the chips and their manufacturers except to say "NXP LPC812", "NXP LPC824", "STM32L031F4P6", and "STMF103xx". See [RFIIE: Low-end ARM MCU with working multi-master I2C peripheral and library](#RFIIE_low_end_arm_mcu_with_working_multi_master_i2c_peripheral_and_library)

To be fair, there were some warning signs. For example, *ST RM0008 Reference manual, August 2017*, despite claiming on page 752, *26.2 I2C main features*:

> * Multimaster capability: the same interface can act as Master or Slave

has on page 766, *26.3.4 Error conditions, Arbitration lost (ARLO):*
> * the I2C Interface goes automatically back to slave mode (the MSL bit is cleared). When the I2C loses the arbitration, it is not able to acknowledge its slave address in the same transfer, but it can acknowledge it after a repeated Start from the winning master.

I'm still willing to believe my failure to get this working was due to  my inability to decipher the (miserable excuses that pass for) documentation and/or  reverse-engineer the chips. But after weeks of effort (not "months", nobody said anything about "months") I broke down and asked two friends, who between them have almost 60 years of embedded programming experience and are among the smartest people I've ever met. Both said essentially the same thing, which was: "I've never heard of anyone using I2C multi-master mode. Given all the bugs that are in these kinds of chips, I'm not surprised that it doesn't work."

Thanks a lot. Could have saved me, uhh, "weeks", of beating my head against the wall/chips. Again in the interest of full disclosure, one of the friends, who, when I started the project and told him, "I'm not afraid of embedded development: I've had to code against some of the worst, buggy, un- and mis-documented software APIs ever written," replied back to me, "The embedded world is worse!"

I didn't believe him at the time.

I was wrong.  
<br>


<a name="required_i2c_features"></a>
Also admittedly, my requirements push I2C multi-master to the limits. The "all nodes receive all messages" and "arbitration is based on node priority"  means that either:

1. All nodes send to, and receive on, the I2C "general call" address, with my scheme's priority in the next, post-address, data byte. This means the arbitration loss and switch-over to receiver ("slave") mode has to take place not on the address byte but on a later one, which is probably even farther down the rabbit hole of I2C edge-case features. Or ...

2. The chip/peripheral needs to support multiple receive addresses. This way the I2C address could be used as my scheme's priority. Some chips implement this, but in limited ways that make them unsuited for my purposes. For example, STMF103xx supports two addresses -- not enough. NXP's LPC824 supports four addresses, again not enough, but one of them can be generalized with a mask of "don't care" bits. (Other chips have this and/or a min-max range of addresses.) That would work, but at least the LPC824 (and others I've looked at) only indicate *that* one of the masked/range of addresses matched and data is being received -- not *which* address, so again are useless for my needs.

Finally ... why not use I2C the way it was intended to be used? One master, multiple slaves, master polls slaves (setting the "read" bit in the address byte) and slaves respond by sending data using the "slave send to master" protocol. One word: [Latency](#latency) (see above). I have lots of nodes and can't afford the latency of round-robin polling each in sequence just to get data from one.


#### Why not bit-bang I2C? <a name="why_not_bit_bang_i2c"></a>

So ... if I2C multi-master is great (design) but fails (implementation), and I'm going to have to write software bit-banging code anyway, why not do it for the well-known, tested, possibly working in some hardware (for compatibility) I2C protocol instead of "rolling" my own?

Good question. Here's the hopefully good answer ...

Again looking at *NXP Semiconductors UM10204 I2C-bus specification and user manual Rev. 6, 4 April 2014*, this time *page 11, Section 3.1.7 Clock synchronization, Fig 7. Clock synchronization during the arbitration procedure*.

Without endlessly quoting from the document (read it -- it's good stuff), clock synchronization is the first part of the arbitration process. When one or more "masters" lower the SCL line to initiate a transaction, all other masters which wish to compete in arbitration detect this and lower their outputs to the SCL line to match.

When each master's SCL LOW time period expires it raises its SCL output. Due to the "wired-AND" nature of the open-drain line, the line doesn't go high until all have raised their outputs. When each master sees the line go high, it begins timing its SCL HIGH period, and, again, when this expires it lowers its output again.

In this way, the LOW period lasts from the first master to go low until the last to go high, and the HIGH period from the last to go high until the first to go low. This achieves the clock synchronization, and logical arbitration based on address bit values is layered on top of this lower-level clock timing.

This works well in hardware, both by virtue of logic speeds (much faster than software) and because the MCU's I2C peripheral subsystem is doing nothing except watching for and executing the protocol.

But consider trying to emulate it in bit-banging software. One or more nodes may be "busy" when the SCL line first goes low -- either due to executing some other code in a polling loop, or interrupt latency (raw latency,  because another interrupt is executing at higher priority, etc). That/those node(s) might respond to the the SCL falling edge late, *after* other, non-slow masters allow the SCL line to go high. The slow nodes could then lower their SCL
outputs on the next (or later) clock cycle without knowing they're late, and put their data bits on the I2C SDA line at the wrong time.

This is the "timing/performance requirement" that tri2b and quad4me were designed to eliminate. Nodes ("masters") can respond to protocol changes on the hardware lines as quickly or slowly as they choose, and the protocols remain in synchronization. See [The Protocols](#the_protocols).


#### What about CAN bus? <a name="what_about_CAN_bus"></a>

Basically another good question. This section is an attempt to answer it proactively.

CAN bus -- in principle -- would solve all my problems. In practice there are reasons why it doesn't. In roughly more-important-to-less order:

1. CAN bus is very poorly supported, especially on low-end, low-pin-count chips. The last time I checked, DigiKey listed only one TSSOP-20 chip with CAN. I've heard this may be due to restrictive licensing issues (hooray for open source, hint, hint).

2. CAN bus is electrical overkill for my application. I need to place approximately 10 to 20 nodes on a 12-to-24 inch long bus. See [RFIIE: GPIO drive capability](#RFIIE_gpio_drive_capability). CAN bus, with its twisted pair lines and balanced drivers is designed for tens of nodes over tens of meters distance.

3. CAN bus (nominally) requires external balanced-line driver chips. I'm aware of the driver-less diode "hack" but don't know how well it works.

4. CAN bus' software protocol is overkill for my needs (error correction, "mailboxes", etc).

5. CAN bus comes in "variants", including STM's "bxCAN". Given my experiences with I2C multi-master implementation failures, this fact does not fill me with confidence regarding the inter-operability of the implementations.

But again, I'm open to suggestions. See ([RFIIE: Low-End ARM MCU with working inter-operable CANbus (and library)](#RFIIE_low_end_arm_mcu_with_working_inter_operable_can_bus_and_library)).

Also any other existing protocols/algorithms. See [RFIIE: Alternative, existing protocol/algorithm](#RFIIE_alternative_existing_protocol_algorithm).

Finally, if this was Stack Overflow, the *second* answer to everything I've written in this section would be, "Why doan u use CANbus, u mow-rhan? Ain't I smart 2 be the 1st wun to post dis?" (The first would be a response to the ["Features"](#features) section, above -- the canonical "Why would you want to do that?") C'mon, GitHub. Get with the program!


#### Why the silly names? <a name="why_the_silly_names"></a>

The original name for the three-line protocol was "tri2c". Stupid pun off some other serial protocol I'd heard of. But, you know ... lawyers and all that. So I came up with "tri2b", as in it "tries to be" a workable protocol.

Due to some development snafus that I don't care to describe (eventually traced to a certain GCC-ARM compiler optimizing out calls to inline functions despite those functions accessing declared-volatile registers) (days, not weeks -- nobody said anything about "weeks") I was for a time convinced that the edge based approach (see [Edge- vs level-based](#edge_vs_level_based)) of tri2b was flawed by design. In desperation I switched development to a new four-line, level-based  protocol and named it "quad4me" because it was the fallback solution "for me".

I have long thought that when it comes to software libraries, the "cuter" the name, the lower the quality. Your mileage may vary.

Or not.




The Protocols  <a name="the_protocols"></a>
-------------

Both protocols, tri2b and quad4me, are software state machines sequenced by multiple open-drain hardware lines which, in combination, comprise a serial communication bus.

Multiple *nodes* ([1](#nodes_vs_masters)) -- running a protocol state machine on an MCU chip -- connect to the bus. Each node connects to each of the bus lines via open-drain GPIO ports.

*Messages*, consisting of a fixed-length number of *arbitration* bits, a fixed-length number of *metadata* bits, and a variable-length number of *data* bits are sent from any node to all other nodes simultaneously. If two nodes attempt to send at the same time, the arbitration bits control which one gains control of the bus.

The bus has one data line and two or more "handshake" lines, at least one of which is held low by all of the nodes at each state machine state. State machine transitions occur when **all** of the nodes raise their outputs to the line high (high impedance). This is the "wired-AND" logic of open-drain lines.

That's it. The whole thing. Implementation is left as an exercise for the reader. Should only take a few hours.

What? You're still here? Alright ... I'll provide some more details ...

<a name="nodes_vs_masters"></a> *(1) "nodes"=="masters" in I2C/SPI nomenclature, but given that tri2b and quad4me have no "slaves" -- all nodes are equal/hierarchy-less -- and because that whole "master/slave" thing is so pre-13th Amendment -- I use the term "node" instead.*


<a name="two_level_state_machine"></a>
### Two-level state machine

(And I thought I could get away without documenting all this. Oh, well.)

The state machines actually consist of two levels:

1. <a name="states"></a> A lower-level set of states, per-line-transition, called simply "States". The states are READ, WRIT ("write"), and in the case of quad4me, NEXT. Each bit of a message requires a transition through each of these states in sequence.

2. <a name="phases"></a> A higher-level set of states called "Phases". Multiple bits, each communicated via the READ/WRITE/(NEXT) States, make up the phases: IDLE, ARBT (arbitration), META (metadata), and DATA (data). The IDLE phase has zero bits, ARBT and META a fixed number, and DATA a variable number (possibly zero) specified by the META bits, (see [meta2bits()](#meta2bits), below)).


### tri2b protocol <a name="tri2b_protocol"></a>

tri2b requires one DATA line, and two handshake lines which I've labeled more-or-less arbitrarily ALRT ("alert") and LTCH ("latch").

The electrical/logical high and low states of the lines, which in turn drive the [State](#states) transitions, look like this:

<a name="tri2b_timing_diagram"></a>

        DATA  :X::?::X::?::X::?::X::?::X::?::X::?::X::?::X::?:::::
              W      W     W     W     W     W     W     W     W 
        ALRT  ~~\___/~\___/~\___/~\___/~\___/~\___/~\___/~\___/~~~
                  R     R     R     R     R     R     R     R     
        LTCH  ___/~\___/~\___/~\___/~\___/~\___/~\___/~\___/~\____

        Legend:
        :    data line, either high or low
        X    data line, data bit written: high->high, high->low, low->high, or low->low
        ?    data line, data bit read
        ~    handshake line high
        _    handshake line low
        /    handshake line, low->high
        \    handshake line, high->low
        R    READ State
        W    WRIT State

A message starts when one or more nodes which have data to send place the first of their arbitration (priority) bits on their DATA ports, then lower their ALRT ports, and finally raise their LTCH ports. When LTCH goes high (wired-AND), all (sending) nodes have placed their data and the DATA line is ready to be read.

When all other nodes detect a falling edge on their ALRT ports, they likewise set their DATA output. If they have data to send and have detected the falling edge (or have been interrupt-triggered by it, see [To interrupt or not to interrupt](#to_interrupt_or_not_to_interrupt), below) before they have initiated the message sequence themselves, they place their first arbitration bit. If they don't have data to send, they raise their DATA output high (see ["non-competing nodes"](#non_competing_nodes), below). They then lower their ALRT output, and raise their LTCH output.

**This is the "clockless" / "no timing requirements" basis of the protocols**. No state transition into READ state can take place until **all** nodes have raised their LTCH ports due to the open-drain, wired-AND nature of the lines.

Every node then reads the input bit on its DATA port, lowers its LTCH port, and finally raises its ALRT port. The ALRT line going high is a signal that all nodes have read the data line, and all can transition to the next WRIT State.

This per-bit sequence continues through the [arbitration](#arbitration_phase) phase and similarly for the metadata and data phase. The only difference is that during the metadata and data phases only one node (the arbitration winner) is placing data bits on the data line -- all others leave their DATA ports high so as not to interfere. (See [The failed promise of hardware swapover](#the_failed_promise_of_hardware_swapover), below.)

Note that the exact order of the above sequences of events -- the raises and lowers of the handshake lines -- is absolutely critical. See [(Almost) Timing-free](#almost_timing_free), below.


### quad4me protocol  <a name="quad4me_protocol"></a>

        DATA  :X::?:::X:?:::X:?:::X:?:::X:?:::X:?:::X:?:::X:?:::::
                    N     N     N     N     N     N     N     N 
        ALRT  ~~\__/~~\__/~~\__/~~\__/~~\__/~~\__/~~\__/~~\__/~~~~
                  R     R     R     R     R     R     R     R     
        LTCH  ___/~~\__/~~\__/~~\__/~~\__/~~\__/~~\__/~~\__/~~\___
               W      W     W     W     W     W     W     W     W
        CYCL  ~~~~\__/~~~\_/~~~\_/~~~\_/~~~\_/~~~\_/~~~\_/~~~\_/~~

        Legend:
        :    data line, either high or low
        X    data line, data bit written: high->high, high->low, low->high, low->low
        ?    data line, data bit read
        ~    handshake line high
        _    handshake line low
        /    handshake line, low->high
        \    handshake line, high->low
        R    READ State
        W    WRIT State
        N    NEXT State


The quad4me protocol is very similar to tri2b with the exception of an additional NEXT State after READ and before WRIT. This extra State is required due to the following ...


### Edge- vs level-based <a name="edge_vs_level_based"></a>

Because State transitions in tri2b are triggered by edges (rising, except for the initial message start falling edge), the handshake lines can be lowered at any time after the rising edge takes place. This freedom does not exist in a level-based protocol such as quad4me.

Consider the LTCH line rising edges in the tri2b [timing_diagram](#tri2b_timing_diagram), above. If this were a level-based protocol, the LTCH line would need to stay high until the ALRT line went high signaling all nodes have read the DATA line and all can transition to WRIT State.

The LTCH line needs to stay high because if not, one or more "fast" nodes could read the data line and lower their LTCH ports (the first one to do so would lower the line due to open-drain wired-AND electrical physics) and one or more "slow" nodes that hadn't seen LTCH high yet would miss their READ states.

So ... why not wait until after ALRT goes high to lower LTCH? That would leave the door open to a different race condition: One or more fast nodes could see the ALRT line high, place their next data bit, lower their ALRT and raise their LTCH ports before one or more of the slow nodes had lowered their LTCH ports. This would cause the LTCH line to go back high signaling that the new data bit was ready to be read when in fact the slow nodes had not yet put their next bits on the DATA line.

This is the reason for the four-line (three handshake plus data) requirement of the level-based quad4me protocol (and, conversely, why the edge-based tri2b needs only two handshake lines). It's also why quad4me is <a name="theoretically_faster"></a> (theoretically -- see [Rise Time](#rise_time), below) 1.5 times slower than tri2b: It requires three States per bit instead of two.

I would be extremely interested in a protocol design that avoids this conundrum. See [RFIIE: A three (or fewer) line level-based protocol](#RFIIE_three_or_fewer_line_level_based_protocol), below.


### Arbitration phase  <a name="arbitration_phase"></a>

In both tri2b and quad4me, arbitration is handled by the well-known "first zero bit wins" algorithm, which leverages the wired-AND logic of open-drain communication lines.

<a name="non_competing_nodes"></a>
In their arbitration phases, nodes running the protocols which have data they wish to send place their arbitration bits, in MSB-to-LSB order, on the DATA line according to the bitwise State protocols described above. Nodes without data to send always place "1" bits and thus do not compete for arbitration.  All nodes read the current arbitration bit on their DATA ports at the appropriate State time.

Due to the wired-AND logic, if **any** node places a "0" bit (lowers the data line), the line will go low regardless of if and how many other nodes are placing "1" bits (logic high, i.e. high-impedance on an open-drain port) at that instant.

Each node compares the current DATA line value (zero or one) to its own current arbitration bit, and if its bit is "1" and the line is "0", drops out of arbitration (loses). For all subsequent bits it places a "1" on the line, as do all non-competing nodes from the beginning MSB.

In this way the node with the lowest arbitration number (lowest number == highest priority) -- the one which has never had a "1" overridden by a "0" --  wins. (Non-competing nodes will always lose by this logic.)

For example:

                       node #5               node #2              node #9                node #3
    Bit    Line   Arbt=5 Write Result   Arbt=2 Write Result   Arbt=9 Write Result   Arbt=3 Write Result
     3       0     0101    0    pend     0010    0    pend     1001    1    lose     0011    0    pend
     2       0     0101    1    lose     0010    0    pend     1001    1    lost     0011    0    pend
     1       1     0101    1    lost     0010    1    pend     1001    1    lost     0011    1    pend
     0       0     0101    1    lost     0010    0    win      1001    1    lost     0011    1    lose

Note that this is **not** the same as the logical AND of the arbitration values:

    0b0101 & 0b0010 & 0b1001 & 0b0011 = 0b0000

In which case a non-existent node 0b0000 would "win".

For an optional enhancement to arbitration process, see [DYNAMIC_RANK](#dynamic_rank), below.



### (Almost) Timing-free <a name=almost_timing_free></a>

#### One line held low at idle <a name="one_line_held_low_at_idle"></a>

As shown in the protocol [timing diagram](#tri2b_timing_diagram) above, in both tri2b and quad4me all handshake lines must be initialized to  known conditions at idle (before a message starts). The ALRT (and CYCL for quad4me) must be high, and the LTCH line low.

This presents two problems, one electrical and one logical. The electrical one is that the low LTCH line consumes power via its pullup resistor (see [RFIIE: Active pullup](#RFIIE_active_pullup), below) whenever the bus is idle, which is likely to be most of the time in a practical application of the protocols.

The logical problem is: How can the line be initialized to its required known-low condition? How can any/all nodes detect that **all** other nodes have lowered their LTCH ports?

The "high" lines are easy -- when they are high, by the definition  of open-drain line electrical physics, all nodes' ports must be high (high-impedance). (Actually it's not quite that trivial -- an uninitialized node might coincidentally be outputting high without truly being ready to start the protocol.)

But the required-low is much harder, if not impossible. Any single node outputting a low will cause the line to be low. In fact, the protocols leverage this fact in that any one (or more) nodes lowering the ALRT line signals a message start. LTCH has to be low so that when it does go high it's certain that all the nodes have set it as such.

[The Example Implementations and Testbed](#the_example_implementations_and_testbed) skirts this problem by simply delaying a known amount of time at system initialization and taking on faith that all nodes have initialized during that period. This is implemented by:

1. Waiting for all communication lines to go high.
2. Waiting a fixed period of time while checking if LTCH has gone low.
3. Lowering LTCH. The first node to do so will break all others out of their wait loops even if their time periods have not expired.
4. Waiting another fixed period of time.

I would be extremely interested in a timing-less solution to this problem.  See [RFIIE: No line low at idle](#RFIIE_no_line_low_at_idle), below. At one point I had a very complex scheme in which each node enumerated itself on the bus, one at a time, in arbitrary order, until all had done so.  In the end the scheme was unreliable because it used (a variation of) the protocol to do the enumeration and  a node coming onto the bus at the wrong time, driving the lines incorrectly,  could break it.

This problem brings to mind the old question: "Quick, grasshopper. What is the sound of one hand clapping?"


#### Rise time <a name="rise_time"></a>

In the glittering pristine crystal palace of logic built by George Boole, the above protocols achieve their goal of timing-free execution. In the dirty real world of noisy electrical voltage levels, not so much.

Consider the timing diagrams [above](#tri2b_timing_diagram). All of them explicitly depend on one State being completely finished before a rising edge (tri2b) or logic high (quad4me) on a line signals a transition to the next State. For example, nodes output data bits on their DATA ports first, and **then** raise their LTCH ports. When the wired-AND LTCH line goes high it's known for certain that the DATA line is ready.

Maybe yes, maybe no.

Due the rise time of signals (determined by the line's resistance-capacitance time constant) the transitions, particularly from low-to-high, are not instantaneous. In fact, there is a trade-off between the power wasted by the open-drain pull-up resistors and the speed of the  rise time. See [RFIIE: Active pullup](#RFIIE_active_pullup), below. 

If all the lines have identical rise times, and if all the GPIO ports read the same value at the same line voltage level and/or register edges at the same place on the rising waveform, having set the DATA line before the LTCH should be sufficient. If not -- as is likely -- workarounds are needed. See [DATA_WAIT_US](#data_wait_us) in the [Build variants](#build_variants), below. This is especially problematic because in the META and DATA phases a node can raise its handshake line output and wait only until it reads that the line has gone high. But in the ARBT phase another node's bit may be low and the line will never go high during that STATE time period. For this reason, a fixed timout has to be implemented.

Also: The tri2b protocol raises and lowers the handshake lines without intermediate states providing delays as in quad4me. See [tri2b waveforms](#tri2b_timing_diagram), above. This may be problematic for real-world (as compared to theoretical) edge detection. The example implementatation provides [MIN_HIGH_US](#min_high_us) to compensate if necessary.



#### The failed promise of hardware swapover <a name="the_failed_promise_of_hardware_swapover"></a>

The tri2b and quad4me protocols were developed due to a (my) failure to get hardware-implemented I2C multi-master arbitration working.

But ... after tri2b/quad4me arbitration takes place, only a single node is sending, and all others receive. Why not, at that time, switch to a much faster (than bit-banging) hardware-implemented standard protocol like I2C/SPI/USART for the metadata and/or data phases? Moreover, if SPI or USART, the hardware GPIO ports could be switched from open-drain to push-pull for much faster rise times and bitrates, and then switched back for the next tri2b/quad4me arbitration.

I tried implementing this idea. A lot. (Uhh, "weeks").  Unfortunately nothing worked, due to both hardware protocol limitations and/or MCU peripheral implementation problems.

All of the hardware protocols require some kind of flow control, either on their own or via separate, out-of-band signaling, because more than one meta/data byte can be required for each message.  USART's RTS/CTS/etc flow control looked promising (both tri2b and quad4me have an extra communication line beyond data and clock that could be used) but unfortunately, despite the fact that everything else in USART is configurable (clock and data polarity, etc) no MCU peripheral supports RTS/CTS being low==true as opposed to the standard high. And low==true is needed to implement the wired-AND, sender sends only when all receivers are ready, logic.

SPI initially looked even better, both for its faster data rates and (in most implementations) ability to send/receive up to 16 bits at a time. But it needs external flow control for more than 16 bits, and at least on the NXP chips requires a hardware line/port to enable slave reception of data. (Why? With all the configurable bits in the peripheral, why not have one that sets "always enabled"?). The 3- and 4-wire tri2b/quad4me interface doesn't have a line to spare -- two are required for flow control handshaking, plus SPI clock and data, so even 4-wire quad4me falls short. (**Five** lines?? Three or four is bad enough!) (And in my specific system I don't have a spare pin to tie permanently high.)

Finally, I2C. Ironic given that if I2C multi-master worked tri2b and quad4me  wouldn't be necessary. I2C "swapover" problems include the fact that even though the "clock stretching" logic is low==true and explicitly supports any of the multiple slaves controlling the flow, the ACK bit on the data line is high==true and thus the converse.

And finally #2: Above and beyond all these theoretical/design problems, in the real  world I found that the MCUs I tried could not reliably switch their ports back and forth at per-message speeds between standard GPIO functionality (for tri2b/quad4me arbitration) and "special purpose" peripherals (I2C/SPI/USART). Pulse glitches on the lines? I don't know -- I have neither a 'scope or a logic analyzer. But I would welcome any ideas on these subjects; see [RFIIE: Hardware swapover](#rfiie_hardware_swapover), below.



#### To interrupt or not to interrupt <a name="to_interrupt_or_not_to_interrupt"></a>

... that is the question.

I hate polled code. I believe all software should be interrupt-driven (or, similarly, be signal-driven when running under an operating system). Preferably with short interrupt system handlers that enqueue instructions into a FIFO which the code's main loop subsequently pops off and executes. 

So why aren't tri2b and quad4me coded on these design principles this repository's [Example Implementations and Testbed](#the_example_implementations_and_testbed).

Basically for performance reasons. Low-end ARM cores are documented to have 12 or more clock cycles of interrupt latency (plus a similar number for return from ISR?). Bit-banging is slow already -- these cycles add up. The example implementation attempts to get the number of clock cycles down in the 200 range, although this is largely driven by [rise time](#rise_time) issues.

My initial implementations of tri2b/quad4me triggered their ISR multiple times per bit (at each [State](#states) transition, above). I quickly found that they ran in an "interrupt storm", never getting back to the main loop until a full message was complete.

Even using one interrupt per message can slow throughput down compared to a polled approach. (Less  so if the protocol's [protocol() method](#protocol_method) is inlined into the applications main loop to avoid function call and return overhead.)

But [latency](#latency) was the primary design requirement for tri2b/quad4me. Due to the ["any slow node can hang the protocol"](#any_node_hangs) nature of the design, an interrupt-driven approach is necessary to insure all nodes run the protocol as expediently as possible. The only exceptions to this would be if the main loop only executed very brief snippets of code per loop before re-polling `protocol()`.

The `protocol()` method in the example implementation can be compiled to execute  either [bit-by-bit or whole-message](#triquad_bit_by_bit_vs_triquad_whole_message). The former, in which it returns when and if a new bit is not immediately on the bus (see [rise time](#rise_time)) is probably only useful if there is a large -- factor of 100  or more -- disparity between different hardware nodes' processing power. Otherwise even the fastest nodes would likely have too few cycles available to perform any useful work, and the call/return overhead (unless inlined) would predominate.

There are some subtleties involved if the example implementation is compiled with [TRIQUAD_POLLING](#triquad_polling_vs_triquad_interrupts). The interrupt handler will trigger on incoming messages, and after doing send any pending messages the client application may have registered but have not been sent yet due to arbitration losses. This of course in addition to receiving any arbitration-winning messages.

But the client app needs to send newly register pending messages, either immediately or at some other time of its choosing. Calling the `protocol()` method directly leaves open possibility getting a falling edge interrupt before the interrupt has been disabled, and recursively entering `protocol()` again.

A number of such race conditions are possible, even if the call to `protocol()` is bracketed by disabling and re-enabling the interrupt. All can be avoided by having the client application invoke `protocol()` by triggering the interrupt via the NVIC ISPR (set pending interrupt) register instead. This is how the example implementation is coded.

And add one more entry to the ["everyone will hate"](#everyone_will_hate) list, below: Long interrupt service handlers. Compiling with TRIQUAD_POLLING will build what is probably one of the longest-running ISRs ever written. I'm on the fence on this one. It's allowed if following the alternate design philosophy of running the entire application in the ISR(s) -- the implementation here is close to that. It's actually more of a hybrid approach where some non-interrupt-driven code remains in the main loop. Opinions, anyone?



### tri2b or quad4me -- which one? <a name="tri2b_or_quad4me_which_one"></a>

The question of which protocol to use comes down to their [Hardware requirements](#hardware_requirements). The most significant factor is that tri2b uses one fewer communication line than quad4me (3 vs 4). It is also theoretically faster (see [above](#theoretically_faster)).

If your hardware supports the [edge detection requirement](#edge_detection_requirement) of tri2b, it is probably the better choice over quad4me. But ... the edge detection must be failure-proof; see [MIN_HIGH_US](#min_high_us), below. In the presence of real-world noise, quad4me is likely more reliable.

Additionally, the current example implementation requires edge detection if compiled to be interrupt-driven (see [TRIQUAD_POLLING vs TRIQUAD_INTERRUPTS](#triquad_polling_vs_triquad_interrupts), below), although this may not be strictly necessary (see [level-based interrupts](#level_based_interrupts), below). If edge detection is required for interrupts, tri2b's requirements will (likely) be already met.



<a name="the_example_implementations_and_testbed"></a>
The Example Implementations and Testbed 
-------------------------------------

This repository contains example implementations of tri2b and quad4me, a functional test program for them, porting layers to two different MCUs, and a primitive build environment, all written in C++.



### Using/porting the code <a name="using_porting_the_code"></a>

This repository contains a large amount of code. See [Repository directories and files](#repository_directories_and_files), below. Fortunately, the codebase can be looked at as a hierarchy of layers, with only a few of them required for integration into an real application.

The actual protocol implementation code is in the [`tri2b` and `quad4me`](#base_implementations) directories.

Code to execute the protocols on particular MCUs is in the [`ports`](#derived_class_methods) directory tree. Unless by coincidence you are using one of the chips ported here, similar code will need to be written.

The `ports` code is configured (peripheral registers, GPIO registers, hardware MCU pins, etc) via [`*_config.hxx`](#config_files) files in the `lpc824` and `stm32f103` example directories. This separation between ported code and configuration files is an implantation choice.

The ported code has dependencies on a large body of utility code (some of which in turn wraps MCU vendor-supplied header files) which are highly idiosyncratic. Feel free to include them in an application (modulo the GPL license -- see [No Warranty](#no_warranty), above) or replace them with supporting code of your own preference.

[The `randomtest`](#the_testbed) directory contains functional test code for the protocols. It can be used as an example for actual application code.

Finally, the repository contains an again highly idiosyncratic build system -- linker scripts, MCU startup code, Makefiles -- that may be used, modified, or replaced.



### The code itself <a name="the_code_itself"></a>

Everyone will hate the C++ code in this repository. "Everyone" includes: <a name="everyone_will_hate"></a>

* [C coders](#c_coders)
* [C++ coders](#c_plus_plus_coders)
* [Assembly coders](#assembly_coders)
* [Python coders](#python_coders)
* [Other coders](#other_coders)

##### C coders <a name="c_coders"></a>
C coders will hate the code because ... C++


##### C++ coders <a name="c_plus_plus_coders"></a>

C++ purists will hate the code because ... 

Where should I start?

1. "'init' methods? INIT METHODS??? That's not RAII!!!" <a name="cpp_haters_1"></a>
2. "Why don't you use the singleton pattern?" <a name="cpp_haters_2"></a>
3. "Why isn't the code templated on word size?" <a name="cpp_haters_3"></a>
4. "Why the crazy duck-typing? Why don't you use virtual inheritance?" <a name="cpp_haters_4"></a>
5. "Static polymorphism? Why don't you use the Curiously Recurring Template Pattern?" <a name="cpp_haters_5"></a>
6. "Multi-line macros to define methods?  Why aren't you using templates? <a name="cpp_haters_6"></a>
7. "Why aren't you using any design patterns?" <a name="cpp_haters_7"></a>
8. "Why aren't you using the STL? Why aren't you using the 'auto' keyword? Why aren't you using range-based 'for' loops?" <a name="cpp_haters_8"></a>

OK, that's enough. I could go on indefinitely.

There are responses to all of these, some of which follow. Not that I expect any of them to lower the level of righteous indignation the code will raise.

[1](#cpp_haters_1)) Many reasons: 

The required ARM pre-main() startup/init code and linker map configuration are complex enough without adding static construction to the mix. 

Many of the post-`main()` `init()` methods have to be done after application-specific MCU initialization has taken place (peripheral initialization, clock speed, etc.) This initialization does **not** belong in the generic ARM startup `init()`, so pre-`main()` construction wouldn't work. In addition, there's what I consider to be a bug in GCC-ARM static construction -- see [RFIIE: GCC-ARM static construction with pointer member variables](#rfiie_arm_gcc_static_construction_with_pointer_member_variables).

[2](#cpp_haters_2)) No thanks. My understanding is there are always edge cases in the singleton pattern. Plus, more importantly: This is a small embedded application, all statically-allocated memory, no `new` or `malloc()`.

[3](#cpp_haters_3)) Large swaths of the code are implicitly written for a 32-bit CPU. Specifically, a 32-bit \*ARM\* CPU. Porting to a different word-size machine would be a complete rewrite. Typedef'ing of the variables would be the least of one's worries.

[4](#cpp_haters_4)) Embedded application. No code space for vtables nor execution time to indirect through them.

[5](#cpp_haters_5)) The CRTP is not really about static polymorphism. Placing the architecture-specific implementations of the base class methods in the derived class file, when there will only ever be one such compiled into the app binary, is a clean, simple solution.

[6](#cpp_haters_6)) Template by function name? Is this possible?

Note that the macro names are "namespaced" by `TRI2B`/`QUAD4ME` prefixes so as not to conflict with other `#defines`. They are also `#undef`'d immediately after use so as to not "escape" into the code which includes them.

BTW, it's my observation that the more "religious" a C++ programmer is about not using the preprocessor, the more likely they are to use large macros themselves when they find them necessary to work out a particularly obfuscated piece of generic programming. Your mileage may vary.

[7](#cpp_haters_7)) Because I'm trying to write clear, efficient, maintainable code. A different objective than obtaining a good grade from a 21st century CompSci professor. ;)

[8](#cpp_haters_8)) I love the STL. No dynamic memory allocation in this embedded application, so it's a non-starter here.

I love/hate the "auto" keyword. Love it for simplifying declaration of iterators to complex classes. Hate it for making code difficult to maintain when trying to find the type of said iterators. Don't need it, nor range-based loops, in a codebase where all of the "for" statements iterate via POD variables.

Like I said: Let the hating begin. Feel free rewrite the code to your up-to-date C++ liking (but see [No Warranty](#no_warranty)). Also see [**Non-** Request](#RFIIE_non_request), below.

Bjarne Stroustrup himself has said:

> *"Within C++, there is a much smaller and cleaner language struggling to get out."* <http://www.stroustrup.com/bs_faq.html#really-say-that>

I'm no Bjarne Stroustrup, and maybe I'm misinterpreting what he wrote, but for many years my line has been:

> My favorite programming language in the C family is one of my own design. Fortunately, any decent C++ compiler will compile it without modification. I call my language "C+=0.5"


##### Assembly coders <a name="assembly_coders"></a>

Hey, assembly coders! I consider myself one of you -- in spirit if not in practice. Assembly (on a machine far too ancient to mention) was my third computer language after Basic and Fortran (did I mention "ancient"?) and my first great love.

But I'm a backslider. I share the current, common belief that "you can't beat the compiler" (except in very limited contexts).

<a name="no_default_switch_case"></a>
One of those contexts, at least with the GCC-ARM compiler, is a C/C++ `switch/case` statement controlled by an `enum` variable, and which has no `default` case. When compiled as a jump table (as opposed to chained "if-else" branches), GCC  always adds a range check on the switch variable before indexing into the jump table.

To illustrate, the following C++ code:

        class SwitchCase {
          public:
            enum class CASES {  // need 5 or more cases to force jump table
              C0 = 0,           // implementation (with -O1 optimization)
              C1,
              C2,
              C3,
              C4,
            };

            void switch_case(const CASES c);
        };
        
        
        void SwitchCase::switch_case(
        const SwitchCase::CASES c)
        {
            switch (c) {         // need at least 5 cases to compile jump table with -O1
                case CASES::C0:
                    asm("nop");
                    break;

                case CASES::C1:
                    asm("nop");
                    break;

                case CASES::C2:
                    asm("nop");
                    break;

                case CASES::C3:
                    asm("nop");
                    break;

                case CASES::C4:
                    asm("nop");
                    break;
            }
        }

compiles and links into:

        void SwitchCase::switch_case(
        const SwitchCase::CASES c)
        {
            switch (c) {         // need at least 5 cases to compile jump table with -O1
        100000c0:       2904            cmp     r1, #4
        100000c2:       d804            bhi.n   100000ce <SwitchCase::switch_case(SwitchCase::CASES)+0xe>
        100000c4:       0089            lsls    r1, r1, #2
        100000c6:       4b06            ldr     r3, [pc, #24]   ; (100000e0 <SwitchCase::switch_case(SwitchCase::CASES)+0x20>)
        100000c8:       585b            ldr     r3, [r3, r1]
        100000ca:       469f            mov     pc, r3
        /usr/local/example/switch_case.cxx:28
                case CASES::C0:
                    asm("nop");
        100000cc:       46c0            nop                     ; (mov r8, r8)
        /usr/local/example/switch_case.cxx:47

                case CASES::C4:
                    asm("nop");
                    break;
            }
        }
        100000ce:       4770            bx      lr
        /usr/local/example/switch_case.cxx:32
                    asm("nop");
        100000d0:       46c0            nop                     ; (mov r8, r8)
        /usr/local/example/switch_case.cxx:33
                    break;
        100000d2:       e7fc            b.n     100000ce <SwitchCase::switch_case(SwitchCase::CASES)+0xe>
        /usr/local/example/switch_case.cxx:36
                    asm("nop");
        100000d4:       46c0            nop                     ; (mov r8, r8)
        /usr/local/example/switch_case.cxx:37
                    break;
        100000d6:       e7fa            b.n     100000ce <SwitchCase::switch_case(SwitchCase::CASES)+0xe>
        /usr/local/example/switch_case.cxx:40
                    asm("nop");
        100000d8:       46c0            nop                     ; (mov r8, r8)
        /usr/local/example/switch_case.cxx:41
                    break;
        100000da:       e7f8            b.n     100000ce <SwitchCase::switch_case(SwitchCase::CASES)+0xe>
        /usr/local/example/switch_case.cxx:44
                    asm("nop");
        100000dc:       46c0            nop                     ; (mov r8, r8)
        /usr/local/example/switch_case.cxx:47
        }
        100000de:       e7f6            b.n     100000ce <SwitchCase::switch_case(SwitchCase::CASES)+0xe>
        100000e0:       1000018c        .word   0x1000018c

with a jump table at 0x1000018c containing:

        switch_case.elf:     file format elf32-littlearm

        Contents of section .rodata:
         1000018c cc000010 d0000010 d4000010 d8000010  ................
         1000019c dc000010                             ....            

        Disassembly of section .rodata:

        1000018c <.rodata>:
        1000018c:       100000cc        .word   0x100000cc
        10000190:       100000d0        .word   0x100000d0
        10000194:       100000d4        .word   0x100000d4
        10000198:       100000d8        .word   0x100000d8
        1000019c:       100000dc        .word   0x100000dc

I'm not sure what the C and C++ standards say about this. They may require that a non-handled case with no default skip the entire body of the switch statement.

But knowing there will be no type-punning (casting an arbitrary value not in the enum class as the switch variable), I'd code this by hand without the range check (`cmp r1, #4` and `bhi.n 100000ce`) before the jump table lookup. Regardless the standards, GCC has so many extension/options that I wish the was one to force this. If there is, I haven't been able to find it (see [RFIIE: GCC-ARM option for switch/case jump table optimization](#RFIIE_gcc_arm_option_for_switch/case_jump_table_optimization)).


##### Python coders <a name="python_coders"></a>

I love Python. I sprinkle it on my cornflakes at breakfast. I use Python whenever I can, i.e. in non-performance-critical applications. I even believe that most performance-critical applications, at least in non-embedded environments, should be written in Python with C/C++ modules implementing the performance-critical sections.

I'm aware of the existence of MicroPython but haven't tried it. My strong suspicion is that it would not "play" for the minimal hardware environments I've targeted tri2b and quad4me at. Maybe the size efficiencies gained by implementing the protocols in byte code would offset the size of the interpreter, and maybe the interpreted code execution is almost as fast as native code. 

Maybe. As the Missourians say, "Show me!". See [RFIIE: MicroPython implementation](#micropython_implementation)


##### Other coders <a name="other_coders"></a>

Java, C#, Ruby, Swift, Visual Basic, the Arduino ecosystem, etc, etc, ...

I have zero knowledge of, and even less interest in, these languages. Feel free to port tri2b/quad4me to any of them (modulo the GPL license -- see [No Warranty](#no_warranty)).



### The example build system <a name="the_example_build_system"></a>

This repository contains code plus a (my) primitive build environment for compiling, linking, and testing it. I sincerely doubt that anyone will find the build environment useful -- if you have a potential use for the implementations you will doubtless have your own such environment. This one is included mainly as a "sanity check" that the complete codebase has been included and is buildable.

The build system is primitive, idiosyncratic, and completely undocumented. I include it here without any intention of supporting it. But see [RFIIE: Build system enhancements](#rfiie_build_system_enhancements) in the event that generosity drives you to offer any suggestions for its improvement.



### Prerequisites/dependencies  <a name="prerequisites_dependencies"></a>

The code in this repository is meant to be built in a Linux environment. Ports to other platforms may be possible.

GNU configure is a wonderful system which I enjoy using. Unfortunately I do not have the time to learn how to construct "configure.in" files from scratch.

Fortunately, the repository's dependencies are few. They are:

1) The GNU Arm Embedded Toolchain This repository's code has currently been tested with the "gcc-arm-none-eabi-8-2018-q4-major" release, <https://launchpad.net/gcc-arm-embedded/+announcements#j15181>. Ports to other compilers may be possible.

2) GNU make

Included in the repository are the files `core_cm0plus.h`, `core_cmInstr.h`, `core_cmFunc.h`, and `system_LPC8xx.h` from ARM Limited, `stm32f103xb.h` from STMicroelectronics, and `LPC8xx.h` from NXP Semiconductors. My reading of their respective copyright notices is that their inclusion here is allowed. I do not understand the text "modified by ARM 02.09.2019" in `LPC8xx.h` which, as of this writing, is still in the future. That's some pro-active bug fixing there. ;)

I will not combine the code into a Debian package, RedHat RPM, or (shudder) Windows Installer executable. It's good thing GitHub is an independent, open-source- and Linux-centric platform. ;)  Note that it also supports downloading the source tree as a ZIP archive.



### Repository directories and files <a name="repository_directories_and_files"></a>

        +-LICENSE.txt
        +-README.html
        +-README.md
        +-build/
        | +-Makefile.base
        | +-Makefile.triquad
        | +-Makefile.triquad_nxp
        | +-Makefile.triquad_stm
        | +-lpc824/
        | | +-quad4me/
        | | | +-Makefile
        | | | +-mcu_config.hxx
        | | | +-quad4me_config.hxx
        | | | +-randomtest.gdb
        | | +-tri2b/
        | |   +-Makefile
        | |   +-mcu_config.hxx
        | |   +-randomtest.gdb
        | |   +-tri2b_config.hxx
        | +-stm32f103/
        |   +-quad4me/
        |   | +-Makefile
        |   | +-mcu_config.hxx
        |   | +-quad4me_config.hxx
        |   | +-randomtest.gdb
        |   +-tri2b/
        |     +-Makefile
        |     +-mcu_config.hxx
        |     +-randomtest.gdb
        |     +-tri2b_config.hxx
        +-buildtest.sh
        +-include/
        | +-arm/
        | | +-cmsis_gcc.h
        | | +-core_cm0plus.h
        | | +-core_cm3.h
        | | +-core_cmFunc.h
        | | +-core_cmInstr.h
        | +-nxp/
        | | +-LPC82x.h
        | | +-LPC8xx.h
        | | +-system_LPC8xx.h
        | +-stm/
        | | +-stm32f103xb.h
        | | +-system_stm32f1xx.h
        | +-util/
        |   +-bitops.hxx
        |   +-lpc8xx.hxx
        |   +-rank_id.hxx
        |   +-stm32f10_12357_xx.hxx
        |   +-sys_tick.hxx
        |   +-xorshift_random.hxx
        +-init/
        | +-lpc8xx_ram_init.c
        | +-stm32f103_ram_init.c
        +-ld/
        | +-lpc824_ram.ld
        | +-stm32f103_ram.ld
        +-ports/
        | +-lpc824/
        | | +-triquad/
        | | | +-quad4me.cxx
        | | | +-quad4me.hxx
        | | | +-quad4me.inl
        | | | +-randomtest_port.inl
        | | | +-tri2b.cxx
        | | | +-tri2b.hxx
        | | | +-tri2b.inl
        | | +-util/
        | |   +-mcu.cxx
        | |   +-mcu.hxx
        | |   +-mrt.cxx
        | |   +-mrt.hxx
        | |   +-sct.cxx
        | |   +-sct.hxx
        | +-stm32f103/
        |   +-triquad/
        |   | +-quad4me.cxx
        |   | +-quad4me.hxx
        |   | +-quad4me.inl
        |   | +-randomtest_port.inl
        |   | +-tri2b.cxx
        |   | +-tri2b.hxx
        |   | +-tri2b.inl
        |   +-util/
        |     +-mcu.cxx
        |     +-mcu.hxx
        |     +-tim.cxx
        |     +-tim.hxx
        |     +-tim_m_s.cxx
        |     +-tim_m_s.hxx
        +-quad4me/
        | +-quad4me_base.cxx
        | +-quad4me_base.hxx
        +-randomtest/
        | +-randomtest.cxx
        +-tri2b/
        | +-tri2b_base.cxx
        | +-tri2b_base.hxx
        +-triquad/
          +-tri_quad.cxx
          +-tri_quad.hxx

In addition to this `README.md`, see comments/documentation in the files themselves.

#### randomtest.cxx

See [The testbed](#the_testbed), below.

Can be compiled to use either tri2b or quad4me protocol.


#### tri_quad.[ch]xx

TriQuad base/interface class for Tri2bBase and Quad4meBase protocol implementations, [directly below](#base_implementations). Data members and methods common to both protocols.


<a name="base_implementations"></a>
#### triquad_base.[ch]xx, quad4me_base.[ch]xx

The protocol implementations.

Contain classes Tri2bBase and Quad4meBase, respectively

<a name="protocol_method"></a>
Client application calls protocol object's `protocol()` method

`protocol()` method returns `true` when message finished, `false` (only if configured with `TRIQUAD_BIT_BY_BIT`, see [Build variants](#build_variants), below) otherwise. Client app calls protocol object's `role()` method to determine whether message was received (arbitration loss), or pending message was sent (arbitration win).

<a name="meta2bits"></a>
Client needs to implement the `meta2bits()` method returning the app-specific number of data bits calculated from the value of the preceding metadata bits. The example implementation uses a simple a one-to-one mapping, `meta2data(int meta){return meta;}`. A real application might map a small number of message types (fewer metadata bits)  to a set of data lengths, `meta2data(int meta){return message_lengths[meta];}`.

see [Build variants](#build_variants), below


####  tri2b.{hxx,inl,cxx} and quad4me.{hxx,inl,cxx} <a name="ported_code"></a>

Ported code

<a name="derived_class_methods"></a>
Duck-typed,  static inheritance/polymorphic concrete classes, derived from `Tri2bBase` and `Quad4meBase`

Implement architecture-specific versions of methods declared in `Tri2bBase` and `Quad4meBase`, including:

* `void reset_delay_start()`
* `bool reset_delay_wait()`
* `void enable_interrupt()`
* `bool alrt()`
* `bool ltch()`
* `bool cycl()` *(quad4me only)*
* `bool data()`
* `void set_alrt()`
* `void set_ltch()`
* `void set_cycl()` *(quad4me only)*
* `void set_data()`
* `void clr_alrt()`
* `void clr_ltch()`
* `void clr_cycl()` *(quad4me only)*
* `void clr_data()`
* `void disble_alrt_fall()` *(only if* `TRIQUAD_INTERRUPTS` *enabled, see [TRIQUAD_POLLING vs TRIQUAD_INTERRUPTS](#triquad_polling_vs_triquad_interrupts), below)*
* `void enable_alrt_fall()` *(only if* `TRIQUAD_INTERRUPTS` *enabled, see [TRIQUAD_POLLING vs TRIQUAD_INTERRUPTS](#triquad_polling_vs_triquad_interrupts), below)*



Also declare and define other architecture-specific methods **not** in Tri2bBase and Quad4meBase


<a name="config_files"></a>
#### tri2b_config.hxx and quad4me_config.hxx

Porting information

Describe hardware environment (GPIO ports, timers, etc.)


#### mcu_config.hxx

CPU and peripheral configuration (CPU speed, peripheral initialization)


#### rank_id.hxx

Least-recently-used reshuffling of arbitration priority-to-node ID mapping

See file for algorithm description.


#### bitops.hxx

Convenience routines

Wrappers, replacements, and/or supplements to CPU bit instructions


#### xorshift_random.hxx

Pseudo-random number generator


#### LPC8xx.h and stm32f10_12357_xx.hxx

Wrappers and convenience/usability extensions to NXP- and STM-provided include files

NXP's `LPC8xx.h` defines peripheral memory map locations and register layouts, but no bitfields within the registers.

STM's `stm32f103xb.h` does define bitfields, but some very poorly. For example `GPIO_CRL_MODE1_0` and `GPIO_CRL_MODE1_1` are simply bits which client code must combine appropriately. The `stm32f10_12357_xx.hxx` wrapper declares meaningful symbolic constants such as `gpio::crl::CONF_OUTPUT_PUSH_PULL`, `gpio::crl::CONF_OUTPUT_OPEN_DRAIN`, etc.


#### mcu.[ch]xx

Architecture-specific MCU CPU and peripheral initialization, driven by `mcu_config.hxx` files

#### mrt.[ch]xx, sct.[ch]xx, tim.[ch]xx, tim_m_s.[ch]xx

Wrapper classes for hardware peripherals (timers)

`TimMS` class implements two 16-bit timers, chained in series


#### lpc824_ram.{init,ld} and stm32f103_ram.{init,ld}

Simple/primitive MCU (pre-main) init scripts and linker memory map configuration files. See [RFIIE: Build system enhancements](#rfiie_build_system_enhancements).


#### Makefile.base

Highly idiosyncratic compilation recipes


#### Makefile.triquad

Settings. See [Build variants](#build_variants), below. Overridable by commandline arguments.


#### Makefile.triquad_nxp and Makefile.triquad_stm

Default architecture-specific settings. Overridable by commandline arguments.


#### randomtest.gdb

GDB functions, for debugging and statistics reporting

See [GDB test environment](#gdb_test_environment), below.



### The testbed <a name="the_testbed"></a>

The application, code contained in `randomtest.cxx`, sends messages of random size (number of data bits) and receives same. It keeps and array of pseudo-random number generators, one for each node in the system, and steps not only its own PNRG but the other nodes' as well, so know what data to expect in each received message. Halts on any error.

Gathers performance statistics; see [GDB test environment](#gdb_test_environment), below.



### Build variants <a name="build_variants"></a>

The example implementation and testbed are highly configurable at build time through a large, multidimensional matrix of options:

        TRIQUAD_TRI2B        |   TRIQUAD_QUAD4ME
        TRIQUAD_BIT_BY_BIT   |   TRIQUAD_WHOLE_MESSAGE
        TRIQUAD_POLLING      |   TRIQUAD_INTERRUPTS 
        DYNAMIC_RANK         |   (not) DYNAMIC_RANK
        TRIQUAD_STATS        |   (no) TRIQUAD_STATS
        TRIQUAD_PROGRESS     |   (no) TRIQUAD_PROGRESS
        RANDOM_DELAY_US > 0  |   RANDOM_DELAY_US == 0
        BUSY_MASK       > 0  |   BUSY_MASK       == 0
        DATA_WAIT_US    > 0  |   DATA_WAIT_US    == 0

That's `2^9 == 512` combinations. Only a small subset have been tested, and even fewer of those extensively.

Probably the most useful (and most tested) are:

             (TRIQUAD_TRI2B   or TRIQUAD_QUAD4ME   )
        with (TRIQUAD_POLLING or TRIQUAD_INTERRUPTS)
        with  DYNAMIC_RANK
        and   TRIQUAD_STATS
        and   RANDOM_DELAY_US > 0
        and   BUSY_MASK       > 0

That's still four full build-plus-test sessions (multiplied by many debugging iterations). I welcome any reports of success or failure with other configurations.


#### Protocol variants <a name="protocol_variants"></a>

##### TRIQUAD_TRI2B vs TRIQUAD_QUAD4ME

Which of the two protocols to build and link.


##### TRIQUAD_BIT_BY_BIT vs TRIQUAD_WHOLE_MESSAGE <a name="triquad_bit_by_bit_vs_triquad_whole_message"></a>

Whether `protocol()` method returns `false` if waiting for bit and true when message finished, or only when message finished (returning `true`).

See [protocol() return after each bit, whether waiting or not](#protocol_return_after_each_bit), below.


##### TRIQUAD_POLLING vs TRIQUAD_INTERRUPTS <a name="triquad_polling_vs_triquad_interrupts"></a>
Polled or interrupt driven invocation of protocol object's `protocol()` method. See [To interrupt or not to interrupt](#to_interrupt_or_not_to_interrupt), above.


##### DATA_WAIT_US<a name="data_wait_us"></a>

Microseconds to wait after setting data line high (if doesn't go high because in arbitration phase and other node(s) holding low). See [Rise time](#rise_time), above.


##### MIN_HIGH_US<a name="min_high_us"></a>

Minimum time (microseconds) for ALRT or LTCH line to be high (tri2b only).





##### TRIQUAD_NUM_NODES and TRIQUAD_ARBT_BITS

Basic configuration.


##### TRIQUAD_ID

Node ID and initial or permanent arbitration priority, depending on whether [DYNAMIC_RANK](#dynamic_rank) is enabled.


##### DYNAMIC_RANK, MIN_RANK, MAX_RANK <a name="dynamic_rank"></a>

Enable or disable, and set range of node IDs to dynamically rank.

See file rank_id.hxx for algorithm description.



##### TRIQUAD_STATS

Enable or disable recording of arbitration win/loss/etc.

If enabled, `scor()` method returns one of:

        enum class Scor {
            PEND = 0,
            RCVR,
            WNNR,
            NZWN,  // non-zero winner (win with _rank != 0)
            LOSR,
        };


#### Testbed (randomtest.cxx) variants <a name="testbed_variants"></a>

##### TRIQUAD_PROGRESS

*Untested variant.* Short-circuit randomtest main loop if TRIQUAD_BIT_BY_BIT and no change in protocol *State* after call to `protocol()`


##### RANDOM_DELAY_US

Minimum number of microseconds between attempts to send message.


##### BUSY_MASK

Mask LSBs of PRNG for length of busy loop to simulate a client being unresponsive to protocol participation.


##### REPORT_AT

Hit GDB statistics dprintf breakpoint every `(REPORT_AT + 1)` messages
Must be `(2^n)-1` value.

See [GDB test environment](#gdb_test_environment), below.


##### RESET_AT

NOT IMPLEMENTED

Reset communication protocol hardware lines every `(REPORT_AT + 1)` messages. Must be `(2^n)-1` value. 

See [One line held low at idle](#one_line_held_low_at_idle), above.



#### Debug variants <a name="debug_variants"></a>

##### STATE_STRINGS

Include `char*` strings matching `State`, `Phase`, and `Role` enums for GDB dprints.

See [GDB test environment](#gdb_test_environment), below.



### GDB test environment <a name="gdb_test_environment"></a>

Remotely debugging an embedded system using GDB running on a host development computer is a dream come true. It performs executable downloading (to flash or RAM), and code debugging (breakpoints, variable/memory/data/object examination, manually setting variables).

One particularly useful feature is the dprintf command. This enables impact- and footprint-less "printing" of information in the embedded app, without the need for printf, RS-232, USB, etc. code in the target app. It's a great (and recently added?) addition/replacement for using the GDB a "commands" directive ending with "continue".

The randomtest.gdb script files contain several useful GDB functions for easily-readable formatted dumps of tri2b/quad4me objects, and several "dprintf" breakpoints for tracing message sends/receives and/or protocol progress. (Admittedly these vastly slow down execution of the embedded code and thus affect timing of the communication line transitions.)

In addition to the dprints, the `results` function collates and presents data collected about the protocol's performance. This is not implemented as a dprintf, in order to allow shelling out the host system's `data` command to track wall-clock time. It is also useful for calling manually in an interactive GDB session after halting the embedded code.

Note also that randomtest.cxx has several variables named "`xxx_K`" which are initialized by compile-time constants (set via the Makefiles) but are not const themselves in order to allow runtime changes in GDB (typically before the start of a test).

In a production system, each node's binary code would be compiled (or likely patched) with the node's fixed ID, and then loaded into MCU flash memory. For development purposes, I load the a single, per-architecture build of the code into RAM, and dynamically set the node IDs in GDB. Something like:

        $ gdb randomtest.elf
        (gdb) source randomtest.gdb
        (gdb) set var randomtest::triquad_random._NODE_ID = 2
        (gdb) set var randomtest::triquad_random._rank    = 2
        (gdb) enable 2
        (gdb) date_continue

The above, again admittedly,  leaves out a fair amount of additional infrastructure (`openocd` scripts for SWD hardware debugging connections and GDB wrappers to connect to `openocd`) that is completely specific to my hardware development environment. I am not including them here because they're outside the scope of this repository. See the "sanity check" comment in [the example build system](#the_example_build_system), above.



### Example implementation enhancements/improvements <a name="enhancements_improvements"></a>

##### Arbitrary-length messages (data longer than 32 bits) <a name="arbitrary_length_messages"></a>

The system I designed tri2b and quad4me for only required 32 bits of data per message at most. Additionally, I wanted to avoid the overhead of shifting incoming bits into multiple bytes or words.

The protocols could be easily extended to handle messages of arbitrary length data.


##### Level-based interrupts <a name="level_based_interrupts"></a>

The interrupt-driven versions of current example implementation trigger on falling edges of the ALRT line. This largely makes moot the quad4me protocol (if interrupt-driven), the main advantage of which over tri2b is that it is purely level-based (see [To interrupt or not to interrupt](#to_interrupt_or_not_to_interrupt), above).

Some ARM MCUs (NXP LPC824, for example) are documented to have optional level-based interrupts. The semantics of these are inherently confusing (does the interrupt fire continuously if the input stays at the configured-to-interrupt level?) and I was not able to easily get it to work in my codebase. Regardless, it may be an interesting option to explore.


##### protocol() always return after each bit <a name="protocol_return_after_each_bit"></a>

The example implementation's [TRIQUAD_BIT_BY_BIT](#triquad_bit_by_bit_vs_triquad_whole_message) variant of the `protocol()` method configures it to return after each bit has been sent or received. It does not, however, return while waiting for the bits (protocol handshakes, see [tri2b protocol](#tri2b_protocol) and [quad4me protocol](#quad4me_protocol), above).

There could be a third option to TRIQUAD_BIT_BY_BIT vs TRIQUAD_WHOLE_MESSAGE <a name="triquad_bit_by_bit_vs_triquad_whole_message"></a> which makes `protocol()` return while waiting. As per [discussion](#to_interrupt_or_not_to_interrupt) above, this would only be of value if a node's CPU performance was much faster than that of the other nodes in the system plus the hardware [rise time](#rise_time) of the communication lines, and it could perform a useful amount work during the brief waits.


##### Inline protocol() method <a name="inline_protocol_method"></a>

If the `protocol()` method is called from only one place in the client application code, it might make sense to force it to be compiled as an inline function despite its very significant size. This would eliminate one function call and return, again likely only of value if doing polling with [TRIQUAD_BIT_BY_BIT](#triquad_bit_by_bit_vs_triquad_whole_message) and/or the `protocol()` method always returned after each bit as per [above](#protocol_return_after_each_bit).


##### Dynamic MCU clock speed <a name="dynamic_mcu_clock_speed"></a>

The protocols' performance is tied to MCU clock speed. (Bit-banging sucks). In a system where communications are idle most of the time -- but latency and/or communication speed are still prime issues -- it might be worthwhile to increase the MCU clock speed when entering the `protocol()` method and return it to its original value on message completion.


##### Oversample lines in quad4me <a name="oversample_lines_in_quad4me"></a>

An earlier revision of the `Quad4me` derived class implementations had a debug option to repeatedly sample the state of the communications lines in the `alrt()`, `ltch()`, `cycl()`, and `data()` methods. This could be restored to improve noise immunity if necessary.




<a name="RFIIE"></a>
Requests For Improvements, Information, and Enhancements (RFIIE)
----------------------------------------------------------------

#### Generic Request <a name="RFIIE_generic_request"></a>
I am interested in any and all improvements to the capabilities, execution speed, and/or compiled code size of this repository's source code as long as they do not significantly obfuscate the code's readability and maintainability.

#### **Non-** Request <a name="RFIIE_non_request"></a>
I am **not** interested in gratuitous rewrites of the implementation code, in particular any modifications or additions to fit it into some pre-conceived software methodology such as design patterns, new features/syntax of C++14,17,20,  doxygen-formatted comments, etc. See [C++ coders](#c_plus_plus_coders), above. Please feel free to fork this repository create a version more to your liking (but respect the [GPL](#no_warranty)).

I am likewise not interested in any modifications to base the code, or make it dependent, on other-party libraries such as opencm3 or, in particular,  STM HAL. If there are any such pull requests that are sufficiently interesting (i.e. contain actual improvements as per [Generic Request](#RFIIE_generic_request), above)) I will decipher and backport them to this codebase. Please consider making my life easier and submit a version with a minimum subset of modifications to the existing code. Thank you.

<a name="RFIIE_code_formatting"></a>
And lay off the indentation and alignment. Quick: In which version is it easier to find the missing "," syntax error?

        ezi2c_random.init(i2c_config::I2C           ,
                          i2c_config::CLCK_SWM_REG  ,
                          i2c_config::DATA_SWM_REG  ,
                          i2c_config::CLOCK_DIVISOR 
                          i2c_config::HGH_CLOCK_BITS,
                          i2c_config::LOW_CLOCK_BITS,
                          i2c_config::CLCK_IOCON_REG,
                          i2c_config::DATA_IOCON_REG,
                          i2c_config::PIN_TYPE       );

or

        ezi2c_random.init(i2c_config::I2C, i2c_config::CLCK_SWM_REG, i2c_config::DATA_SWM_REG, i2c_config::CLOCK_DIVISOR i2c_config::HGH_CLOCK_BITS, i2c_config::LOW_CLOCK_BITS, i2c_config::CLCK_IOCON_REG, i2c_config::DATA_IOCON_REG, i2c_config::PIN_TYPE);


I rest my case. A friend of mine brilliantly calls this "visual grep".



#### Specific Requests <a name="specific_requests"></a>

##### RFIIE: Low-end ARM MCU with working multi-master I2C peripheral and library <a name="RFIIE_low_end_arm_mcu_with_working_multi_master_i2c_peripheral_and_library"></a>
A low-end (20-32 pins, TSSOP-20 QFN-33, 8-16 KB RAM, 16-32 KB flash, c. $2 USD in single-piece quantities) ARM chip with a working multi-master I2C peripheral. See [Required I2C Features](#required_i2c_features), above. Register-level example code  (no other-party libraries required) if at all possible.

Point me to such a system and I'll drop all this tri2b and quad4me nonsense.


##### RFIIE: Low-End ARM MCU with working inter-operable CAN bus (and library) <a name="RFIIE_low_end_arm_mcu_with_working_inter_operable_can_bus_and_library"></a>
Similar requirements to [RFIIE: Low-End ARM MCU With Working Multi-master I2C peripheral and library](#RFIIE_low_end_arm_mcu_with_working_multi_master_i2c_peripheral_and_library), above. 


##### RFIIE: Alternative, existing protocol/algorithm  <a name="RFIIE_alternative_existing_protocol_algorithm"></a>
An existing, published, open-source protocol/algorithm that implements the [features](#features), above. I've searched and haven't found one besides I2C, SPI, CAN bus, RS-485, and others that don't. Hardware support would be a significant plus.


##### RFIIE: Fewer lines  <a name="RFIIE_fewer_lines"></a>
A way to provide the capabilities of tri2b with fewer than three hardware lines, or quad4me with fewer than four


##### RFIIE: A three (or fewer) line level-based protocol  <a name="RFIIE_three_or_fewer_line_level_based_protocol"></a>
A level-based design similar to the edge-based tri2b that runs on two (or one) handshake lines (total of three or two including data line).


##### RFIIE: No line low at idle  <a name="RFIIE_no_line_low_at_idle"></a>
A way to provide the capabilities of tri2b or quad4me without requiring one or more hardware lines to be held low (dissipating pull-up resistor power) at idle.


##### RFIIE: Hardware support  <a name="RFIIE_hardware_support"></a>
A hardware-supported implementation of tri2b or quad4me on existing MCU hardware, such as NXP's "Pattern match engine" (NXP Semiconductors *UM10800, LPC82x User manual, Rev. 1.2, 5 October 2016*, p.133, *10.5.2 Pattern match engine*).

Note that a full hardware implementation, either on a standalone programmable-logic chip or as a built-in peripheral to a new MCU is unlikely to be of value as one might as well implement working I2C multi-master capability (the timing/performance requirements of I2C being easy to match at logic speeds, and because the hardware would be 100% devoted to the task -- see [Why Not Bit-Bang  I2C?](#why_not_bit_bang_i2c), above). 

But alternately, if I2C is subject to the same glitch failures as tri2b, a hardware implementation of quad4me might be more reliable and therefor worthwhile. See ["tri2b or quad4me -- which one?'](#tri2b_or_quad4me_which_one), above.


##### RFIIE: Hardware swapover  <a name="rfiie_hardware_swapover"></a>
A working implementation in which tri2b or quad4me is used for arbitration, followed by some hardware-implemented protocol such as SPI/USART/I2C for metadata and data, and a final swap back to GPIO functionality for the next tri2b/quad4me message/arbitration. See [The failed promise of hardware swapover](#the_failed_promise_of_hardware_swapover), above.


##### RFIIE: tri2b/quad4me ports  <a name="tri2b_quad4me_ports"></a>

Additional, chip-specific derived classes porting Tri2bBase and Quad4meBase to specific ARM MCUs. See [Ported code](#ported_code), above.

I myself will likely be adding one specific variant of the STM32F7 line  in the near future.



##### RFIIE: GPIO drive capability  <a name="RFIIE_gpio_drive_capability"></a>
Any pointers on how many nodes can be put on a single tri2b/quad4me multi-line bus, and how physically long that bus can be? This in light of the drive capabilities of MCU GPIO ports.

EE's are like cops: They're never around when you need one. ;)


##### RFIIE: Active pullup  <a name="RFIIE_active_pullup"></a>
I have seen circuit designs which attempt to get around the trade-off inherent with pull-up resistors for open-drain lines: The lower the resistance the faster the rise time on the line but at the cost of more wasted power dissipation. See [Rise Time](#rise_time), above. The circuits seem to work by detecting the voltage level on the line: When the line is near but not at zero volts the circuit switches in a parallel low-value resistor tied to Vdd to improve rise time, and then switches it back out when the voltage approaches Vdd leaving a higher-valued resistor to hold the line high without excess dissipation.

Are there any off-the-shelf chips that implement this, likely with additional features and capabilities (multiple pull-up speeds, latchup protection, etc)? Note that this solution is preferable to I2C buffer/driver chips, as those require a chip/channel for each chip/pin's connection to a communication line compared to one chip/circuit per line for active pullups.


##### RFIIE: GCC-ARM option for switch/case jump table optimization <a name="RFIIE_gcc_arm_option_for_switch/case_jump_table_optimization"></a>
A GCC-ARM option to eliminate the range check before jump table in a jump-table-implemented, default-less switch/case statement using an enum variable. See [above](#no_default_switch_case). Adding the `-Wswitch-enum` flag doesn't help. I have only found a request dated 2004 for this on the GCC sites, with no followups afterwards.


##### RFIIE: MicroPython implementation <a name="micropython_implementation"></a>
Implementations of tri2b and quad4me in MicroPython, without significant impact in code size (including addition of the interpreter) and execution speed.


##### RFIIE: GCC-ARM static construction with pointer member variables <a name="rfiie_arm_gcc_static_construction_with_pointer_member_variables"></a>
GCC-ARM (at least through version 8) does not place static objects with `constexpr` constructors with pointer arguments/members into the "D" initialized data segment, even if those pointers are statically-known at compile time and the member variables they initialize are const. Instead, it places them, uninitialized, into the "B" section and generates constructor code and calls to same.

        $ cat constexpr_constructor.cxx
        #include <stdint.h>

        typedef struct {
            volatile uint32_t register1;
            volatile uint32_t register2;
        } RegistersTypedef;

        #define R (RegistersTypedef*)0x12345678

        class C {
          public:
            constexpr C(
            RegistersTypedef* const initializer)
            : initialized(initializer)
            {}
          protected:
            RegistersTypedef* const initialized;
        };

        C c(const_cast<RegistersTypedef*const>(R));

        int main()
        {
        }


GCC-ARM (at least through GCC-ARM 8) compiles and links this as:

        $ nm -C constexpr_constructor.o
        00000074 t _GLOBAL__sub_I_c
        0000001c t __static_initialization_and_destruction_0(int, int)
        00000000 W C::C(RegistersTypedef*)
        00000000 W C::C(RegistersTypedef*)
        00000000 n C::C(RegistersTypedef*)
        00000000 B c
        00000000 T main

GCC 4.8.5 on Intel Linux gives:

        $ nm -C constexpr_constructor.o
        0000000000000000 D c
        0000000000000000 T main

Is there any way to force GCC-ARM to do the same?


##### RFIIE: GCC-ARM -fshort-enums and ARM byte vs word access speed <a name="RFIIE_GCC-ARM_fshort_enums_and_ARM_byte_vs_word_access_speed"></a>
ARM documentation for e.g. the M0+ and M3 cores seems to indicate that there is no difference in memory access speed (load/store RISC architecture) between 8-bit bytes and 32-bit words. GCC-ARM's documentation for the `-fshort-enums` flag (enum storage is the minimum size that will contain all the enum values) seems to say otherwise, and in fact I have not been able to get `-fshort-enums` to take effect if `-O1` (or higher?) optimization is in place, as shown by the following compile-time test:

        enum class State {
            READ = 0,
            WRIT = 1,
            NEXT = 2,
        };
        
        static_assert(sizeof(State) == 1, "Compile with -fshort-enums")


##### RFIIE: Build system enhancements <a name="rfiie_build_system_enhancements"></a>
I would welcome any improvements, fixes, pointing out of errors, etc. to my primitive build system ... as long as they are not major rewrites or replacements for it. Suggestions to use OpenCM3, ARM DS-5, Keil MDK, Atollic TrueSTUDIO, IAR Workbench, etc. don't count.
