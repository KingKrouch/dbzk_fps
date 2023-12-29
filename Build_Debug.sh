#!/usr/bin/env bash
cmake -B Intermediate/Debug -G Ninja -DCMAKE_BUILD_TYPE=Debug && cd Intermediate/Debug && ninja