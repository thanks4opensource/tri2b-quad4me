#!/bin/sh

HERE=$PWD

for dir in \
    build/lpc824/tri2b  \
    build/lpc824/quad4me \
    build/stm32f103/tri2b \
    build/stm32f103/quad4me \
; do
    cd $dir

    pwd
    make clean && \
    make \
    INTRPT_POLL=POLLING \
    TRIQUAD_PROTOCOL=-DTRIQUAD_BIT_BY_BIT \
    PROGRESS=-DTRIQUAD_PROGRESS \
    WARNINGS_FLAG='-Wextra -Wswitch --warn-no-return-type -Werror'
    if [ $? != 0 ] ; then pwd ; exit ; fi
    pwd
    echo -e \\n\\n\\n

    pwd
    make clean && \
    make \
    INTRPT_POLL=POLLING \
    TRIQUAD_PROTOCOL=-DTRIQUAD_WHOLE_MESSAGE \
    PROGRESS=-UTRIQUAD_PROGRESS \
    WARNINGS_FLAG='-Wextra -Wswitch --warn-no-return-type -Werror'
    if [ $? != 0 ] ; then pwd ; exit ; fi

    pwd
    echo -e \\n\\n\\n
    pwd
    make clean && \
    make \
    INTRPT_POLL=INTERRUPTS \
    TRIQUAD_PROTOCOL=-DTRIQUAD_WHOLE_MESSAGE \
    PROGRESS=-UTRIQUAD_PROGRESS \
    STATS=-UTRIQUAD_STATS \
    WARNINGS_FLAG='-Wextra -Wswitch --warn-no-return-type -Werror'
    if [ $? != 0 ] ; then pwd ; exit ; fi
    pwd
    echo -e \\n\\n\\n

    pwd
    echo -e \\n\\n\\n
    pwd
    make clean && \
    make \
    INTRPT_POLL=INTERRUPTS \
    TRIQUAD_PROTOCOL=-DTRIQUAD_WHOLE_MESSAGE \
    PROGRESS=-UTRIQUAD_PROGRESS \
    WARNINGS_FLAG='-Wextra -Wswitch --warn-no-return-type -Werror'
    if [ $? != 0 ] ; then pwd ; exit ; fi
    pwd
    echo -e \\n\\n\\n

    cd $HERE
done
