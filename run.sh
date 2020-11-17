#!/bin/bash

rm -f -r Outputs
mkdir Outputs
chmod g+rwx Outputs
dir="/u/css/classes/3481/194/lab11/Tests"

tests=( error1 error2 error3 error4 error5 error6
        error7 error8 error9 error10 error11 error12 
        error13 error14 error15 nop irmovq 
        addq andq subq xorq addqfor1 addqfor2 addqfor3
        addqfor4 addqfor5 addqfor6 rrmovq cmovle cmovl
        cmove cmovne cmovge cmovg mrmovq1 mrmovq2
        pushq1 pushq2 popq1 popq2 pushqpopq rmmovq
        memfor1 memfor2 except1 except2 except3 except4 except5 except6
        except7 loaduse1 loaduse2 jmp je jg jge jl jle jne
        addOne sum jecall
      )

numTests=0
numPasses=0

for atest in ${tests[@]}
do
   infile="$dir/$atest.yo"
   studoutfile="$atest.sdump"
   instoutfile="$dir/$atest.idump"
   rm -f $studoutfile
   ./yess $infile > $studoutfile
   rm -f diffs
   diff $instoutfile $studoutfile > diffs
   if [ ! -e $studoutfile ] ||  [ ! -s $studoutfile ] || [ -s diffs ]; then
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
   chmod g+rw Outputs/*
else
   rm -f -r Outputs
fi 

