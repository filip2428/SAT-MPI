#!/bin/sh
instances=${1:-1}
seed=${2:-0}
for solver in resolution dp dpll cdcl grasp; do
    ./$solver "$instances" "$seed"
done
