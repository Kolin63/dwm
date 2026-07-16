#!/usr/bin/env bash

if lsof +D ~/Videos 2>/dev/null | grep -q obs; then
  obs=" Recording "
fi

player_title=$(playerctl metadata -f "{{title}}")
player_artist=$(playerctl metadata -f "{{artist}}")
player_name=$(playerctl metadata -f "{{playerName}}")
player_prefix=""
if [[ "$player_name" == "spotify" ]]; then
  player_prefix="\x09"
elif [[ "$player_name" == "chromium" || "$player_name" == "firefox" ]]; then
  player_prefix="\x0b󰖟"
fi
if [[ ! -z "$player_artist" ]]; then
  player_artist=" - $player_artist"
fi
if [[ ! -z "$player_title" ]]; then
  player="$player_prefix ${player_title}${player_artist} "
fi

mouse_status=$(/home/colin/.dotfiles/scripts/mousetoggle get)
if [[ "$mouse_status" == "0" ]]; then
  mouse="󰍾 Off"
else
  mouse="󰍽 On"
fi

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
  if   [ $battery_percentage -le 10 ]; then
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
  if [[ "$battery_state" == "charging" ]]; then
    battery=" $battery"
  fi
  battery+=" $battery_time_to_empty$battery_time_to_full"
fi

volume_raw=$(pamixer --get-volume)
if   [ $(pamixer --get-mute) == "true" ]; then
  volume="  0% "
elif [ $volume_raw -eq 00 ]; then
  volume="  $volume_raw% "
elif [ $volume_raw -le 50 ]; then
  volume="  $volume_raw% "
else
  volume="  $volume_raw% "
fi

date=$(date +"%a %b %d %Y ")
time=$(date +"%I:%M %p")

status+="\x06 $obs"
status+="$player"
status+="\x03 $mouse"
status+="\x03 $battery"
status+="\x0a $volume"
status+="\x03 $date"
status+="\x08 $time"
xsetroot -name "$(echo -e "$status")"
