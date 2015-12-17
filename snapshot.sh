rm -rf primary_dump
rm -rf secondary_dump
rm sec_op.log
mkdir primary_dump
mkdir secondary_dump
./mongodump_secondary --oplog --out secondary_dump > sec_op.log
ts=$(cat sec_op.log)
array=( $ts )
sec="${array[0]}"
inc="${array[1]}"
./mongodump_primary --host rs0/node2.mongodb-sv-fl.uw-cs739-f2015.emulab.net:27017 --oplog --ts $sec:$inc --out primary_dump 
