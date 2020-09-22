#!/bin/bash

rm -f -r Outputs
mkdir Outputs
dir="/u/css/classes/3481/194/lab6/Tests"
tests=( asumr error1 error2 error3 error4 error5 error6
        error7 error8 error9 error10 error11 error12 
        error13 error14 error15 )

numTests=0
numPasses=0

for atest in ${tests[@]}
do
   infile="$dir/$atest.yo"
   studoutfile="$atest.sdump"
   instoutfile="$dir/$atest.idump"
   rm -f $studoutfile
   ./lab6 $infile > $studoutfile
   rm -f diffs
   diff -b $instoutfile $studoutfile > diffs
   if [ -s diffs ]; then
      cp $instoutfile Outputs/
      cp $infile Outputs/
      mv $studoutfile Outputs/
      echo "Testing $infile ... failed"
      cat diffs
      rm -f diffs
   else
      rm -f diffs
      rm -f $studoutfile
      echo "Testing $infile ... passed"
      numPasses=$(($numPasses+1))
   fi
   numTests=$(($numTests+1))
done
echo " "
echo "$numPasses passed out of $numTests tests."

if [ $numPasses -ne $numTests ]; then
   echo "Files saved in your Outputs directory."
   echo "Input files end with a .yo."
   echo "Your output files end with a .sdump."
   echo "Correct output files end with a .idump."
else
   rm -f -r Outputs
fi 

