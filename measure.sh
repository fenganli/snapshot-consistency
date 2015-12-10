./insert_read_backup 4
# now bring up the back up process
./backup &
# backup=$!
# now measure the affected insert, read time
./affected_insert_read 3
# kill -KILL $backup
