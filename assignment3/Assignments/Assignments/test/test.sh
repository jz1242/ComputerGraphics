#!/bin/bash

rayFiles=("test_triangle" "test" "test_point_light" "test_spot_light" "test_texture_square" "test_texture_sphere") 
rayFiles+=("dog" "bunny" "dragon")
rayFiles+=

width=500  
height=500
runtests=false

while getopts "t w:h:" opt; do
  case $opt in
    t )
        runtests=true
        echo Generating the "test" images. 
        ;;
    h )
        height=$OPTARG
        ;;
    w )
        width=$OPTARG
        ;;
    \?)
        echo script usage: $(basename $0) "[-t] (runs tests) [-w height] [-h height] [-r rLimit] [-c cutoff]"
        exit 1
        ;;
  esac
done

if [ "$runtests" = true ]
then
    for i in "${rayFiles[@]}" 
    do
        echo -e Running $i.ray ...
        ./Assignment3 --in "$i.ray" --width $width --height $height
        echo ""
    done
fi

echo -e Running art.ray ...
./Assignment2 --in art.ray --width 1500 --height 1500 
