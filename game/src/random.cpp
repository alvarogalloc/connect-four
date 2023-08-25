module;
#include <random>
export module random;

export
{
  template<typename T>
  concept arithmetic = std::integral<T> || std::floating_point<T>;

  template<arithmetic T> T random(T min, T max)
  {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    if constexpr (std::integral<T>) {
      std::uniform_int_distribution<T> dis(min, max);
      return dis(gen);
    } else if constexpr (std::floating_point<T>) {
      std::uniform_real_distribution<T> dis(min, max);
      return dis(gen);
    }
  }
}
