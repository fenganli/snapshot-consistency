./insert_read_backup 5
# now bring up the back up process
./backup &
backup=$!
# now measure the affected insert, read time
./affected_insert_read 5
kill -KILL $backup
