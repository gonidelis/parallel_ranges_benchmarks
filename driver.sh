# Copyright (c) 2021 Giannis Gonidelis

for i in {15..29}
do
    ITER=$((2 ** $i))
    ./bin/transform --algo transform --n $ITER | tee -a result.csv
done