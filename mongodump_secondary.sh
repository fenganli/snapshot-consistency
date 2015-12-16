ssh node1 'cd $HOME; mongodump --oplog --quiet'
echo "bsondump on oplog"
ssh node1 'cd dump; bsondump oplog.bson > /users/fli69/oplog.json; echo "head"; head -2 /users/fli69/oplog.json; echo "tail"; tail -2 /users/fli69/oplog.json' > oplog.output
echo "bsondump on restaurants data"
ssh node1 'cd dump/test; bsondump restaurants.bson > /users/fli69/restaurants.json; echo "head"; head -2 /users/fli69/restaurants.json; echo "tail"; tail -2 /users/fli69/restaurants.json' > restaurants.output
