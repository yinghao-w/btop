#!/bin/sh

dir=$(dirname $0)
$CC --shared -fPIC -o $dir/librocm_smi64.so $dir/mock_rocm.c
$CC --shared -fPIC -o $dir/libnvidia-ml.so $dir/mock_nvml.c
export LD_LIBRARY_PATH=$dir
$dir/../bin/btop
unset LD_LIBRARY_PATH
