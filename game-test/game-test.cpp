#include <boost/ut.hpp>
int main () {
  using namespace boost::ut;
  "board"_test = [] {
    expect(42_i == 41);
  };
}
