#include <iostream>
#include "linal.h"

using linal::Vector;


bool initialize_test() {
  Vector<int> first(1, 2, 3);
  
  bool result = true;
  if (first.x() != 1 || first.y() != 2 || first.z() != 3) {
    std::cerr << "Did not initialize properly!" << std::endl;
    result = false;
  }
  return result;
}

bool check_equals_test() {
  Vector<int> first(1, 0, 0);
  Vector<int> first_equal(1, 0, 0);
  Vector<int> second(0, 1, 0);

  bool result = true;
  if (!(first == first_equal)) {
    std::cerr << "Equal vectors are not equal!(==)" << std::endl;
    result = false;
  }

  if (first != first_equal) {
    std::cerr << "Equal vectors are not equal!(!=)" << std::endl;
    result = false;
  }

  if (first == second) {
    std::cerr << "Not equal vectors are equal!(==)" << std::endl;
    result = false;
  }

  if (!(first != second)) {
    std::cerr << "Not equal vectors are equal!(!=)" << std::endl;
    result = false;
  }

  return result;
}

int main(void) {
  std::cout << "Started testing..." << std::endl;
  int num_passed_tests = 0;
  int num_tests = 2;
  num_passed_tests += initialize_test();
  num_passed_tests += check_equals_test();

  std::cout << num_passed_tests << " of " << num_tests <<
      " tests passed" <<  std::endl;
}
