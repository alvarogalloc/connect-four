#include <boost/ut.hpp>
int main () {
  using namespace boost::ut;
  // TODO
  "board"_test = [] {
    expect(42_i == 41);
  };
}
