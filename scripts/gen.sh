#!/usr/bin/env bash
SCRIPT_DIR=`dirname $0`
DATA_DIR=${SCRIPT_DIR}/../data

${SCRIPT_DIR}/gen_long_bit_string.py > ${DATA_DIR}/leetcode-add-binary.txt
${SCRIPT_DIR}/gen_long_string.py > ${DATA_DIR}/leetcode-longest-substr.txt
${SCRIPT_DIR}/gen_int_seq.py > ${DATA_DIR}/leetcode-int-seq.txt

cd ${DATA_DIR} >/dev/null
for f in *.zip;do
	unzip -x $f
done
cd - >/dev/null

