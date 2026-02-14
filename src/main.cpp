#include "../include/cpu.hpp"
#include "../include/memory.hpp"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc < 3) {
    std::cerr << "Missing parameters\n";
    std::exit(EXIT_FAILURE);
  }
  std::string path = argv[2];
  std::string filetype = argv[1];
  Memory m;
  try {
    if (filetype == "-e") {
      m.load_elf(path);
    } else {
      m.load_binary(path);
    }
    CPU c{&m};
    while (!c.stopped) {
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
