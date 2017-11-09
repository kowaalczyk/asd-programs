#!/bin/bash

if [[ $# -gt 2 ]]; then
  app=$2
  testPath=$3
  params=$1
else
  app=$1
  testPath=$2
  params=""
fi

echo "This script will only report failed tests."
echo "Running tests..."

for inFile in $testPath/*.in;
do
  fileName=$(echo $inFile | cut -f 1 -d '.')
  cat $inFile | $app $params 1> ${fileName}.realout 2> ${fileName}.realerr
  #sprawdzamy czy program sie poprawnie wykonal
  if [[ $? != 0 ]]; then
    echo "EXECUTION FAILURE: ${fileName}"
  fi
  #porownojemy wyjscie z poprawnym wynikiem
  diff $fileName.out $fileName.realout > ${fileName}.diffout
  if [[ $? != 0 ]]; then
    echo "INCORRECT STD. OUTPUT: ${fileName}"
    #w przypadku niepoprawnego wyniku nie usuwamy go
  else
     rm ${fileName}.diffout
  fi
  #sprawdzamy stderr
  if [[ $# -gt 2 ]]; then
    diff $fileName.err $fileName.realerr > ${fileName}.differr
    if [[ $? != 0 ]]; then
      echo "INCORRECT ERR. OUTPUT: ${fileName}"
      #w przypadku niepoprawnego wyniku nie usuwamy go
    else
       rm ${fileName}.differr
    fi
  fi
done

rm $testPath/*.realout $testPath/*.realerr
