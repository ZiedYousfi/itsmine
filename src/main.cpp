#include <iostream>

class hey {
 private:
  std::string name;
  int age;
  int* p_height;

 public:
  hey(std::string n, int a, int* h) : name(n), age(a), p_height(h) {};
  ~hey() { delete p_height; };

  void printInfo() {
    std::cout << "Name: " << name << ", Age: " << age
              << ", Height: " << *p_height << std::endl;
  }

  void setName(std::string n) { name = n; }
  void setAge(int a) { age = a; }
  void setHeight(int* h) {
    if (p_height) {
      delete p_height;
    }
    p_height = h;
  }
};

int main(int argc, char** argv) {
  std::cout << "Hello, from itsmine!\n";

  hey* h = new hey("Alice", 30, new int(170));
  h->printInfo();
  delete h;
  return 0;
}
