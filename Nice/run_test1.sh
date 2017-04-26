#!/bin/bash
# Run several workers in parallel

bin/worker 0 &
bin/worker 4 &
