#!/bin/bash
echo "WARNING: realclean will remove the hershey font C declarations in gen_c_src."
echo "WARNING: You will have to use GNU Octave if you want to regenerate them."

read -p "Are you sure you want to remove gen_c_src? (y/n) " -n 1 -r
echo ""
if [[ $REPLY =~ ^[Yy]$ ]]
then
    rm -rf gen_c_src
    echo "removed gen_c_src"
fi
