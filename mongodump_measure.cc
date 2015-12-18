#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <memory>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/types.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>

#define BILLION 1000000000L

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::open_document;
using bsoncxx::builder::stream::close_document;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using bsoncxx::builder::stream::finalize;

std::string exec(const char* cmd);

int main(int argc, char** argv) {
    mongocxx::instance inst{};
    mongocxx::client conn{mongocxx::uri{}};

    auto db = conn["test"];
    
    int max_insert_size;
    int str_size;
    int interval; // milli second
    if (argc <= 1) {
	std::cout << "There are not enough command line arguments" << std::endl;
    } else {
	max_insert_size = atoi(argv[1]);
        str_size = atoi(argv[2]);
    }
    
    std::vector<double> backup_time; 

    // We first drop the whole test collection, if not create a collection.
    std::string street_name = std::string(1024*1024*str_size, 'a');

      		auto restaurant_doc = document{} << "address" << open_document << "street"
                                     << street_name
                                     << "zipcode"
                                     << "10075"
                                     << "building"
                                     << "1480"
                                     << "coord" << open_array << -73.9557413 << 40.7720266
                                     << close_array << close_document << "borough"
                                     << "Manhattan"
                                     << "cuisine"
                                     << "Italian"
                                     << "grades" << open_array << open_document << "date"
                                     << bsoncxx::types::b_date{12323} << "grade"
                                     << "A"
                                     << "score" << 11 << close_document << open_document << "date"
                                     << bsoncxx::types::b_date{121212} << "grade"
                                     << "B"
                                     << "score" << 17 << close_document << close_array << "name"
                                     << "Vella"
                                     << "restaurant_id"
                                     << "41704620" << finalize;
    		auto res = db["restaurants"].insert_one(restaurant_doc); 
     db["restaurants"].drop();
     sleep(10);
    
    struct timespec start, end;
    for (int j = 0; j < max_insert_size; j++) {
    		auto restaurant_doc = document{} << "address" << open_document << "street"
                                     << street_name
                                     << "zipcode"
                                     << "10075"
                                     << "building"
                                     << "1480"
                                     << "coord" << open_array << -73.9557413 << 40.7720266
                                     << close_array << close_document << "borough"
                                     << "Manhattan"
                                     << "cuisine"
                                     << "Italian"
                                     << "grades" << open_array << open_document << "date"
                                     << bsoncxx::types::b_date{12323} << "grade"
                                     << "A"
                                     << "score" << 11 << close_document << open_document << "date"
                                     << bsoncxx::types::b_date{121212} << "grade"
                                     << "B"
                                     << "score" << 17 << close_document << close_array << "name"
                                     << "Vella"
                                     << "restaurant_id"
                                     << "c" + std::to_string(j) << finalize;
    		auto res = db["restaurants"].insert_one(restaurant_doc); 
      }

      
      clock_gettime(CLOCK_MONOTONIC, &start);	
      std::string result = exec("bash /users/fli69/snapshot-consistency/newmongodump_measure.sh");
      clock_gettime(CLOCK_MONOTONIC, &end);
      double passed_time = ((double)BILLION*end.tv_sec+end.tv_nsec-(double)BILLION*start.tv_sec-start.tv_nsec)*1.0/BILLION;
      backup_time.push_back(passed_time); 
      std::cout << "backup time:" << passed_time << std::endl;

      return 0;
}

std::string exec(const char* cmd) {
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) return "ERROR";
    char buffer[128];
    std::string result = "";
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}
