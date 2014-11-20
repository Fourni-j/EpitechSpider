tmp="$1"
size=${#tmp}

boucle=$((3-(size/9)))

while [ $boucle -ge 0 ]
do
    echo -en "\t"
    boucle=$(($boucle - 1))
done
    
echo -e "\033[32mDone\033[0m"
