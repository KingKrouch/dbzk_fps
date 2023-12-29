#!/usr/bin/env bash
cmake -B Intermediate/Release -G Ninja -DCMAKE_BUILD_TYPE=Release && cd Intermediate/Release && ninja