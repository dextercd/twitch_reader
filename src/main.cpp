#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>

#include "twitch_chat_reader.hpp"

int main(int argc, char** argv)
{
  if(argc != 2) {
    std::cerr << "Need channel to track!\n";
    return -1;
  }

  std::ifstream user_file{"user_data.txt"};
  std::string name;
  std::string oauth;

  std::getline(user_file, name);
  std::getline(user_file, oauth);

  asio::io_service io_service;
  client c{io_service, name, oauth, argv[1]};

  std::thread threads[1];
  for(auto& t : threads) {
    t = std::thread([&io_service]() {
      try {
        io_service.run();
      } catch(const std::exception& e) {
        std::cout << "exception: " << e.what() << '\n';
      } catch(...) {
        std::cout << "unknown exception.\n";
      }
    });
  }

  char ew;
  while(std::cin >> ew) { // allows the user to press C^D or C^Z to close the program
  }

  for(auto& t : threads) {
    t.join();
  }
}
