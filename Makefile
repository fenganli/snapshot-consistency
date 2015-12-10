compile:
	c++ --std=c++11 insert_read_backup.cc -o insert_read_backup $$(pkg-config --cflags --libs libmongocxx)
	c++ --std=c++11 backup.cc -o backup
	c++ --std=c++11 affected_insert_read.cc -o affected_insert_read $$(pkg-config --cflags --libs libmongocxx)
	c++ --std=c++11 secondary_measure.cc -o secondary_measure $$(pkg-config --cflags --libs libmongocxx)

measure:
	./measure.sh	

clean:
	rm -rf affected_insert_read
	rm -rf backup
	rm -rf insert_read_backup
	rm -rf secondary_measure
	rm -rf dump
