#include <exception>
#include <iostream>

void runPressureHistoryTests();
void runSeaLevelTests();
void runTrendTests();
void runZambrettiTests();

int main() {
  try {
    runPressureHistoryTests();
    runSeaLevelTests();
    runTrendTests();
    runZambrettiTests();
  } catch (const std::exception &ex) {
    std::cerr << "Test failure: " << ex.what() << '\n';
    return 1;
  }

  std::cout << "All domain tests passed\n";
  return 0;
}
