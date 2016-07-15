#!/bin/bash

BASEDIR=$(dirname "$0")
APP="$BASEDIR/build/src/frontend/rdjs"
TEST_FILES="$BASEDIR/tests/resources"

if [ ! -f $APP ]; then
    echo "Projeto precisa ser compilado para rodar os testes"
    exit 0
fi


for entry in $TEST_FILES/*
do
  echo "Running $entry ..."
  $APP < "$entry"
  echo ""
done
