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
                                     << "b" + std::to_string(j) << finalize;
    		auto res = db["restaurants"].insert_one(restaurant_doc);
	  }
     }
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
