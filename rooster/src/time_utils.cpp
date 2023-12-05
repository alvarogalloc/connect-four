module;

#include <chrono>
#include <cstdint>
#include <thread>
export module rooster:time_utils;

export
{
  auto now() { return std::chrono::steady_clock::now(); }
  auto elapsed(std::chrono::steady_clock::time_point start)
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(now() - start).count();
  }
  void sleep_ms(std::uint32_t ms) { std::this_thread::sleep_for(std::chrono::milliseconds(ms)); }
}
