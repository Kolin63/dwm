#!/usr/bin/env bash

battery_raw=$(upower -i /org/freedesktop/UPower/devices/battery_BAT0)
battery_present=""
battery_state=""
battery_time_to_empty=""
battery_time_to_full=""
battery_percentage=""

if [[ $battery_raw =~ present:[[:space:]]*([[:alnum:]]+) ]]; then
  battery_present="${BASH_REMATCH[1]}"
fi
if [[ $battery_raw =~ state:[[:space:]]*([[:alpha:]-]+) ]]; then
  battery_state="${BASH_REMATCH[1]}"
fi
if [[ $battery_raw =~ time\ to\ empty:[[:space:]]*([0-9]+\.[0-9]+|[0-9]+)[[:space:]]+([[:alpha:]]+) ]]; then
  battery_time_to_empty="${BASH_REMATCH[1]} ${BASH_REMATCH[2]}"
fi
if [[ $battery_raw =~ time\ to\ full:[[:space:]]*([0-9]+\.[0-9]+|[0-9]+)[[:space:]]+([[:alpha:]]+) ]]; then
  battery_time_to_full="${BASH_REMATCH[1]} ${BASH_REMATCH[2]}"
fi
if [[ $battery_raw =~ percentage:[[:space:]]*([0-9]+)% ]]; then
  battery_percentage="${BASH_REMATCH[1]}"
fi

if [[ "$battery_present" == "yes" ]]; then
  if [[ "$battery_state" == "charging" ]]; then
    battery="󰂄 $battery_percentage%"
  elif [ $battery_percentage -le 10 ]; then
    battery="󰁺 $battery_percentage%"
  elif [ $battery_percentage -le 20 ]; then
    battery="󰁻 $battery_percentage%"
  elif [ $battery_percentage -le 30 ]; then
    battery="󰁼 $battery_percentage%"
  elif [ $battery_percentage -le 40 ]; then
    battery="󰁽 $battery_percentage%"
  elif [ $battery_percentage -le 50 ]; then
    battery="󰁾 $battery_percentage%"
  elif [ $battery_percentage -le 60 ]; then
    battery="󰁿 $battery_percentage%"
  elif [ $battery_percentage -le 70 ]; then
    battery="󰂀 $battery_percentage%"
  elif [ $battery_percentage -le 80 ]; then
    battery="󰂁 $battery_percentage%"
  elif [ $battery_percentage -le 90 ]; then
    battery="󰂂 $battery_percentage%"
  elif [ $battery_percentage -le 100 ]; then
    battery="󰁹 $battery_percentage%"
  fi
  battery+=" $battery_time_to_empty$battery_time_to_full"
fi

volume_raw=$(amixer -D pulse get Master)
volume_perc=""
if [[ $volume_raw =~ \[([0-9]+)%\] ]]; then 
  volume_perc="${BASH_REMATCH[1]}"
fi
if   [ $volume_perc -eq 00 ]; then
  volume="  $volume_perc"
elif [ $volume_perc -le 50 ]; then
  volume="  $volume_perc"
else
  volume="  $volume_perc"
fi

date=$(date +"%a %b %d %Y")
time=$(date +"%I:%M")

status+="\x03 $battery  "
status+="\x03 $volume%  "
status+="\x03 $date  "
status+="\x08 $time  "
xsetroot -name "$(echo -e $status)"
