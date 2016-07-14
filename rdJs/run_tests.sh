#!/bin/bash

APP="build/src/frontend/rdjs"
TEST_FILES="tests/resurces"

for entry in $TEST_FILES/*
do
  echo "Running $entry ..."
  $APP < "$entry"
  echo ""
done
