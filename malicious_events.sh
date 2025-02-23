#!/bin/bash

# Define auditd rules 
# -w /bin/firefox -p x -k firefox_exec
# -w /bin/apt -p x -k program_apt_install
# -w /bin/dpkg -p x -k program_dpkg_install
# -w /etc/shadow -p rwax -k shadow_actions

declare -i USAGE_AMOUNT=$((1 + $RANDOM % 5))

echo $USAGE_AMOUNT
NULLIFY_OUTPUT=" &>>/dev/null"

for i in $(seq 1 $USAGE_AMOUNT)
do
    COMMAND="cat /etc/shadow"
    COMMAND+=$NULLIFY_OUTPUT
    eval $COMMAND
done

for i in $(seq 1 $USAGE_AMOUNT)
do
    COMMAND="apt --help"
    COMMAND+=$NULLIFY_OUTPUT
    eval $COMMAND
done

for i in $(seq 1 $USAGE_AMOUNT)
do
    COMMAND="apt --help"
    COMMAND+=$NULLIFY_OUTPUT
    eval $COMMAND
done

for i in $(seq 1 $USAGE_AMOUNT)
do
    COMMAND="dpkg --help"
    COMMAND+=$NULLIFY_OUTPUT
    eval $COMMAND
done

for i in $(seq 1 $USAGE_AMOUNT)
do
    COMMAND="firefox --headless &>>/dev/null && pgrep firefox | xargs kill"
    COMMAND+=$NULLIFY_OUTPUT
    eval $COMMAND
done

for i in $(seq 1 $USAGE_AMOUNT)
do
    COMMAND="modprobe dummy_hcd && modprobe -r dummy_hcd"
    COMMAND+=$NULLIFY_OUTPUT
    eval $COMMAND
done

