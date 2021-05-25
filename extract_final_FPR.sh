keywords="TPR"
folder="final-FPR"
csvname="csv/final_FPR.csv"

rm -f ./$csvname
for value in "begin_line" 1.0 0.9999 0.95 0.9 0.8 0.7
do
    if [[ $value == "begin_line" ]]
    then 
        echo -n "Range" >> ./$csvname
        for set in 1 20 40 60
        do
            echo -n ",SET $set" >> ./$csvname
        done
        echo -ne "\n" >> ./$csvname
    else 
        echo -n "$value" >> ./$csvname
        for set in 1 20 40 60
        do
            output=$(cat ./$folder/$value-$set.txt | grep "TPR" | tail -1 |awk '{print $2}')
            echo -n ",$output"  >> ./$csvname
        done
        echo -ne "\n" >> ./$csvname 
    fi
done
