pdsh -R ssh -w ^machines 'cd; mongod --shutdown --dbpath mongodb/data' &
echo "shutting down mongodb on node 2"
cd $HOME
mongod --shutdown --dbpath mongodb/data > /dev/null 2>&1 &
