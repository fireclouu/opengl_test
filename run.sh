#!/bin/bash
exec_array=()
opt=1
BUILD_DIR=build
BIN_DIR=$BUILD_DIR/bin
PARENT_DIR=$PWD
TERMUX_X11_ACTIVITY="am start com.termux.x11/.MainActivity"
TERMUX_X11_ACTIVITY_KILL="am broadcast -a com.termux.x11.ACTION_STOP -p com.termux.x11"
TERMUX_X11_ACTIVITY_FLAG=0

mkdir -p $BIN_DIR

for arg in "$@"; do
  if [ $arg == "--termux-x11-support" ]; then
    TERMUX_X11_ACTIVITY_FLAG=1
  fi
done

echo -e "\e[033mRun CMake...\e[0m"
cmake -S . -B $BUILD_DIR > /dev/null
if [ $? -ne 0 ]; then exit 1; fi
echo -e "\e[033mRun Makefile...\e[0m"
make -C $BUILD_DIR > /dev/null
if [ $? -ne 0 ]; then exit 1; fi
echo -e "\e[032mBuilds OK!\e[0m"

for filename in $BIN_DIR/*; do
  if [ -x $filename ] && [ -f $filename ]; then
    exec_array+=("$filename")
  fi
done

while true; do
  echo
  echo "Choose program to run: "
  for ((i = ${#exec_array[@]} - 1; i >=0; i--)); do
    echo -ne "\e[033m$((${#exec_array[@]}-${i})): \e[0m"
    echo "${exec_array[$i]}" | awk -F/ '{print $3}'
  done
  echo -e "\e[033m0: \e[0mExit"
  echo

  read -p "Your choice: " opt
  if ! [[ "$opt" =~ ^[0-9]+$ ]]; then
    echo -e "\e[031mInvalid choice!\e[0m"
    continue
  fi

  if [[ "$opt" -eq 0 ]]; then
    break
  fi

  if [[ "$opt" -gt ${#exec_array[@]} ]]; then
    echo -e "\e[031mInvalid choice!\e[0m"
    continue
  fi

  # inverted sort so the newer exercises will appear on first choice
  selected=$((${#exec_array[@]}-$opt))
  echo -ne "\e[032mSelected "
  echo -n "${exec_array[$selected]}" | awk -F/ '{print $3}'
  echo -e "\e[0m"

  echo "Current Directory will set to $PARENT_DIR/$BIN_DIR"
  cd $PARENT_DIR/$BIN_DIR

  # Termux
  if [ $TERMUX_X11_ACTIVITY_FLAG -eq 1 ]; then
    eval "$TERMUX_X11_ACTIVITY"
  fi

  echo -e "\e[033mTransferring to Program...\e[0m"
  eval "$PARENT_DIR/${exec_array[$selected]}"
  STATUS=$?

  # Termux
  if [ $TERMUX_X11_ACTIVITY_FLAG -eq 1 ]; then
    eval "$TERMUX_X11_ACTIVITY_KILL"
  fi

  echo -e "\e[033mTransferring back to shell...\e[0m"

  if [ $STATUS -eq 0 ]; then
    echo -e "\e[032mProgram exited without errors. "
  else
    echo -e "\e[031mProgram exited with failure ($STATUS). "
  fi
  echo -ne "\e[0m"

  echo "Current Directory will set to $PARENT_DIR"
  cd $PARENT_DIR

  sleep 1
done

echo -e "\e[033mGood bye.\e[0m"
