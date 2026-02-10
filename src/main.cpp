#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "No file specified\n";
    std::exit(EXIT_FAILURE);
  }
  std::string path = argv[1];
  Memory m;
  CPU c{&m};
  try {
    m.load_binary(path);
    int temp = 1000;
    while (!c.stopped && temp--) {
      c.cycle();
    }
  } catch (const std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
  } catch (const std::out_of_range &e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "error unknown" << std::endl;
  }
}
