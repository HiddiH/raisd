for value in 1.0 0.9999 0.95 0.9 0.8 0.7
do
    echo current value is: $value
    make clean > /dev/null
    if [[ $value == 1.0 ]]
    then
        echo making with original code
        make THRESHOLD=$value ORIGINAL_CODE=1 &> /dev/null
    else
        echo making with edited code
        make THRESHOLD=$value ORIGINAL_CODE=0 &> /dev/null
    fi
    for set in 1 20 40 60
    do
        rm -f ./accuracy/$value-$set.txt ./success-rate/$value-$set.txt ./initial-FPR/$value-$set.txt ./final-FPR/$value-$set.txt
        echo testing accuracy with SET $set
        ./RAiSD -n test-run-accuracy -I ./BASE$set.out -L 100000 -T 50000 -f > ./accuracy/$value-$set.txt
        echo testing success with SET $set
        ./RAiSD -n test-run-success-rate -I ./BASE$set.out -L 100000 -T 50000 -d 1000 -f > ./success-rate/$value-$set.txt
        echo testing FPR with SET $set
        FPR=$(./RAiSD -n test-run-fpr -I ./BASE$set.out -L 100000 -k 0.05 -f | grep "FPR Threshold" | awk '{print $3}')
        echo $FPR > ./initial-FPR/$value-$set.txt
        ./RAiSD -n test-run-tpr -I ./TEST$set.out -L 100000 -l $FPR -f > ./final-FPR/$value-$set.txt
    done
done
echo extracting results to csv files
/bin/bash ./extract_accuracy.sh
/bin/bash ./extract_success_rate.sh
/bin/bash ./extract_initial_FPR.sh
/bin/bash ./extract_final_FPR.sh