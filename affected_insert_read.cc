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
    if (argc <= 1) {
	std::cout << "There are not enough command line arguments" << std::endl;
    } else {
	max_insert_size = atoi(argv[1]);	
    }
    std::vector<double> insert_time;
    std::vector<double> read_time;
    // We first drop the whole test collection.
    // db["restaurants"].drop();
    
    for (int i = 0; i <= max_insert_size; i++) {
	int insert_size = (int)(pow(2, i)*1024*3.185);
	// measure the insert time
        struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	for (int j = 0; j < insert_size; j++) {
    		auto restaurant_doc = document{} << "address" << open_document << "street"
                                     << "2 Avenue"
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
	  }
          clock_gettime(CLOCK_MONOTONIC, &end);
          double passed_time = ((double)BILLION*end.tv_sec+end.tv_nsec-(double)BILLION*start.tv_sec-start.tv_nsec)*1.0/BILLION;
	  insert_time.push_back(passed_time);

	  // Now measure read time
          struct timespec read_start, read_end;
          clock_gettime(CLOCK_MONOTONIC, &read_start);
          auto cursor = db["restaurants"].find({});
          int read_count = 0;
	  for (auto&& doc : cursor) {
              read_count++;
          }
          clock_gettime(CLOCK_MONOTONIC, &read_end);
	  passed_time = ((double)BILLION*read_end.tv_sec+read_end.tv_nsec-
		(double)BILLION*read_start.tv_sec-read_start.tv_nsec)*1.0/BILLION;
          read_time.push_back(passed_time);      
           
          db["restaurants"].drop(); // Drop the table 
    }
    std::cout << "Affected insert time: k MB, sec" << std::endl;
    for (int i = 0; i <= max_insert_size; i++)
        std::cout << insert_time[i] << std::endl;
    std::cout << "Affected read time: k MB, sec" << std::endl;
    for (int i = 0; i <= max_insert_size; i++)
        std::cout << read_time[i] << std::endl;
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
