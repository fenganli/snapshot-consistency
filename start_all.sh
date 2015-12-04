pdsh -R ssh -w ^machines 'cd; mongod --shutdown --dbpath mongodb/data;  mongod --replSet "rs0" --dbpath mongodb/data/ > /dev/null 2>&1 ' &
echo "shutting down mongodb on node 2"
cd 
echo "starting mongodb on node 2"
mongod --shutdown --dbpath mongodb/data > /dev/null 2>&1 &
mongod --replSet "rs0" --dbpath mongodb/data/ > /dev/null 2>&1 &
