module;
#include <charconv>
#include <memory>
#include <string>
#include <string_view>
#include <array>
#include <functional>
#include <exception>
#include <optional>
#include <sstream>
#include <iostream>
#include <cassert>
#include <iomanip>

#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
export module centurion;
export {
  
namespace cen::detail {
struct sdl_deleter final
{
  void operator()(void* ptr) noexcept { SDL_free(ptr); }
};
}
namespace cen {
template <typename T>
struct deleter;
template <>
struct deleter<SDL_Window> final
{
  void operator()(SDL_Window* window) noexcept { SDL_DestroyWindow(window); }
};
template <>
struct deleter<SDL_Renderer> final
{
  void operator()(SDL_Renderer* renderer) noexcept { SDL_DestroyRenderer(renderer); }
};
template <>
struct deleter<SDL_RWops> final
{
  void operator()(SDL_RWops* ops) noexcept { SDL_RWclose(ops); }
};
template <>
struct deleter<SDL_Surface> final
{
  void operator()(SDL_Surface* surface) noexcept { SDL_FreeSurface(surface); }
};
template <>
struct deleter<SDL_Texture> final
{
  void operator()(SDL_Texture* texture) noexcept { SDL_DestroyTexture(texture); }
};
template <>
struct deleter<SDL_PixelFormat> final
{
  void operator()(SDL_PixelFormat* format) noexcept { SDL_FreeFormat(format); }
};
template <>
struct deleter<SDL_Palette> final
{
  void operator()(SDL_Palette* palette) noexcept { SDL_FreePalette(palette); }
};
template <>
struct deleter<SDL_Cursor> final
{
  void operator()(SDL_Cursor* cursor) noexcept { SDL_FreeCursor(cursor); }
};
template <>
struct deleter<SDL_Joystick> final
{
  void operator()(SDL_Joystick* joystick) noexcept { SDL_JoystickClose(joystick); }
};
template <>
struct deleter<SDL_GameController> final
{
  void operator()(SDL_GameController* controller) noexcept
  {
    SDL_GameControllerClose(controller);
  }
};
template <>
struct deleter<SDL_Haptic> final
{
  void operator()(SDL_Haptic* haptic) noexcept { SDL_HapticClose(haptic); }
};
template <>
struct deleter<SDL_Sensor> final
{
  void operator()(SDL_Sensor* sensor) noexcept { SDL_SensorClose(sensor); }
};
template <>
struct deleter<SDL_mutex> final
{
  void operator()(SDL_mutex* mutex) noexcept { SDL_DestroyMutex(mutex); }
};
template <>
struct deleter<SDL_sem> final
{
  void operator()(SDL_sem* semaphore) noexcept { SDL_DestroySemaphore(semaphore); }
};
template <>
struct deleter<SDL_cond> final
{
  void operator()(SDL_cond* cond) noexcept { SDL_DestroyCond(cond); }
};
template <>
struct deleter<Mix_Music> final
{
  void operator()(Mix_Music* music) noexcept { Mix_FreeMusic(music); }
};
template <>
struct deleter<Mix_Chunk> final
{
  void operator()(Mix_Chunk* chunk) noexcept { Mix_FreeChunk(chunk); }
};
template <>
struct deleter<TTF_Font> final
{
  void operator()(TTF_Font* font) noexcept { TTF_CloseFont(font); }
};
template <typename T>
using managed_ptr = std::unique_ptr<T, deleter<T>>;
class simd_block final
{
 public:
  using size_type = std::size_t;
  explicit simd_block(const size_type size) noexcept : mData{SDL_SIMDAlloc(size)} {}
  [[nodiscard]] auto data() noexcept -> void* { return mData.get(); }
  [[nodiscard]] auto data() const noexcept -> const void* { return mData.get(); }
  explicit operator bool() const noexcept { return mData != nullptr; }
 private:
  struct simd_deleter final
  {
    void operator()(void* ptr) noexcept { SDL_SIMDFree(ptr); }
  };
  std::unique_ptr<void, simd_deleter> mData;
};
}
namespace cen {
struct version final
{
  int major{};
  int minor{};
  int patch{};
};
[[nodiscard]] constexpr auto current_version() noexcept -> version
{
  return {7, 3, 0};
}
[[nodiscard]] constexpr auto version_at_least(const int major,
                                              const int minor,
                                              const int patch) noexcept -> bool
{
  return (((7)*1'000) + ((3)*100) + (0)) >= (((major)*1'000) + ((minor)*100) + (patch));
}
[[nodiscard]] constexpr auto sdl_version() noexcept -> SDL_version
{
  return {SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_PATCHLEVEL};
}
[[nodiscard]] inline auto sdl_linked_version() noexcept -> SDL_version
{
  SDL_version version{};
  SDL_GetVersion(&version);
  return version;
}
[[nodiscard]] constexpr auto sdl_image_version() noexcept -> SDL_version
{
  return {SDL_IMAGE_MAJOR_VERSION, SDL_IMAGE_MINOR_VERSION, SDL_IMAGE_PATCHLEVEL};
}
[[nodiscard]] inline auto sdl_image_linked_version() noexcept -> SDL_version
{
  const auto* version = IMG_Linked_Version();
  assert(version);
  return *version;
}
[[nodiscard]] constexpr auto sdl_mixer_version() noexcept -> SDL_version
{
  return {SDL_MIXER_MAJOR_VERSION, SDL_MIXER_MINOR_VERSION, SDL_MIXER_PATCHLEVEL};
}
[[nodiscard]] inline auto sdl_mixer_linked_version() noexcept -> SDL_version
{
  const auto* version = Mix_Linked_Version();
  assert(version);
  return *version;
}
[[nodiscard]] constexpr auto sdl_ttf_version() noexcept -> SDL_version
{
  return {SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_PATCHLEVEL};
}
[[nodiscard]] inline auto sdl_ttf_linked_version() noexcept -> SDL_version
{
  const auto* version = TTF_Linked_Version();
  assert(version);
  return *version;
}
}
namespace cen {
using usize = std::size_t;
using uint = unsigned int;
using ulonglong = unsigned long long;
using uint8 = Uint8;
using uint16 = Uint16;
using uint32 = Uint32;
using uint64 = Uint64;
using int8 = Sint8;
using int16 = Sint16;
using int32 = Sint32;
using int64 = Sint64;
using unicode_t = uint16;
using unicode32_t = uint32;
template <typename T>
using seconds = std::chrono::duration<T>;
template <typename T>
using millis = std::chrono::duration<T, std::milli>;
template <typename T>
using minutes = std::chrono::duration<T, std::ratio<60>>;
using u16ms = millis<uint16>;
using u32ms = millis<uint32>;
using u64ms = millis<uint64>;
inline constexpr bool is_debug_build = true;
inline constexpr bool is_release_build = false;
inline constexpr bool on_msvc = false;
inline constexpr bool on_gcc = true;
inline constexpr bool on_clang = true;
template <typename T>
using enable_for_pointer_t = std::enable_if_t<std::is_pointer_v<T>, int>;
template <typename T, typename... Args>
using enable_for_convertible_t =
    std::enable_if_t<(std::is_convertible_v<T, Args> || ...), int>;
template <typename T>
using enable_for_enum_t = std::enable_if_t<std::is_enum_v<T>, int>;
template <typename T, usize Size>
using bounded_array_ref = T (&)[Size];
template <typename T, enable_for_pointer_t<T> = 0>
using owner = T;
template <typename T, enable_for_pointer_t<T> = 0>
using maybe_owner = T;
template <typename T>
using maybe = std::optional<T>;
inline constexpr auto nothing = std::nullopt;
template <typename T>
inline constexpr bool is_number =
    !std::is_same_v<T, bool> && (std::is_integral_v<T> || std::is_floating_point_v<T>);
class exception : public std::exception
{
 public:
  exception() noexcept = default;
  explicit exception(const char* what) noexcept : mWhat{what ? what : "?"} {}
  [[nodiscard]] auto what() const noexcept -> const char* override { return mWhat; }
 private:
  const char* mWhat{"?"};
};
class sdl_error final : public exception
{
 public:
  sdl_error() noexcept : exception{SDL_GetError()} {}
  explicit sdl_error(const char* what) noexcept : exception{what} {}
};
class img_error final : public exception
{
 public:
  img_error() noexcept : exception{IMG_GetError()} {}
  explicit img_error(const char* what) noexcept : exception{what} {}
};
class ttf_error final : public exception
{
 public:
  ttf_error() noexcept : exception{TTF_GetError()} {}
  explicit ttf_error(const char* what) noexcept : exception{what} {}
};
class mix_error final : public exception
{
 public:
  mix_error() noexcept : exception{Mix_GetError()} {}
  explicit mix_error(const char* what) noexcept : exception{what} {}
};
class result final
{
 public:
  constexpr result(const bool success) noexcept
      : mSuccess{success}
  {}
  [[nodiscard]] constexpr explicit operator bool() const noexcept { return mSuccess; }
 private:
  bool mSuccess{};
};
inline constexpr result success{true};
inline constexpr result failure{false};
[[nodiscard]] inline auto to_string(const result result) -> std::string
{
  return result ? "success" : "failure";
}
inline auto operator<<(std::ostream& stream, const result result) -> std::ostream&
{
  return stream << to_string(result);
}
[[nodiscard]] constexpr auto operator==(const result a, const result b) noexcept -> bool
{
  return a.operator bool() == b.operator bool();
}
[[nodiscard]] constexpr auto operator!=(const result a, const result b) noexcept -> bool
{
  return !(a == b);
}
class sdl_string final
{
 public:
  explicit sdl_string(owner<char*> str) noexcept : mStr{str} {}
  [[nodiscard]] auto get() const noexcept -> const char* { return mStr.get(); }
  [[nodiscard]] auto copy() const -> std::string
  {
    if (mStr) {
      return std::string{get()};
    }
    else {
      return std::string{};
    }
  }
  explicit operator bool() const noexcept { return mStr != nullptr; }
 private:
  std::unique_ptr<char, detail::sdl_deleter> mStr;
};
template <typename To, typename From>
[[nodiscard]] constexpr auto cast(const From& from) noexcept -> To
{
  return static_cast<To>(from);
}
template <typename Enum, enable_for_enum_t<Enum> = 0>
[[nodiscard]] constexpr auto to_underlying(const Enum value) noexcept
    -> std::underlying_type_t<Enum>
{
  return static_cast<std::underlying_type_t<Enum>>(value);
}
template <typename T>
[[nodiscard]] constexpr auto isize(const T& container) noexcept(noexcept(container.size()))
    -> int
{
  return static_cast<int>(container.size());
}
[[nodiscard]] inline auto str_or_na(const char* str) noexcept -> const char*
{
  return str ? str : "null";
}
namespace literals {
inline namespace time_literals {
[[nodiscard]] constexpr auto operator""_ms(const ulonglong ms) -> u32ms
{
  return u32ms{static_cast<uint32>(ms)};
}
}
}
}
namespace cen::detail {
template <typename T>
[[nodiscard]] constexpr auto clamp(const T& value,
                                   const T& min,
                                   const T& max) noexcept(noexcept(value < min)) -> T
{
  assert(min <= max);
  if (value < min) {
    return min;
  }
  else if (max < value) {
    return max;
  }
  else {
    return value;
  }
}
template <typename T>
[[nodiscard]] constexpr auto(min)(const T& a, const T& b) noexcept(noexcept(a < b)) -> T
{
  return (a < b) ? a : b;
}
template <typename T>
[[nodiscard]] constexpr auto(max)(const T& a, const T& b) noexcept(noexcept(a < b)) -> T
{
  return (a < b) ? b : a;
}
[[nodiscard]] constexpr auto lerp(const float a, const float b, const float bias) noexcept
    -> float
{
  return (a * (1.0f - bias)) + (b * bias);
}
template <typename T = int>
[[nodiscard]] auto stoi(const std::string_view str, const int base = 10) noexcept(on_msvc)
    -> maybe<T>
{
  T value{};
  const auto* begin = str.data();
  const auto* end = begin + str.size();
  const auto [ptr, err] = std::from_chars(begin, end, value, base);
  if (ptr == end && err == std::errc{}) {
    return value;
  }
  else {
    return nothing;
  }
}
[[nodiscard]] inline auto cmp(const char* a, const char* b) noexcept -> bool
{
  if (a && b) {
    return std::strcmp(a, b) == 0;
  }
  else {
    return false;
  }
}
[[nodiscard]] inline auto address_of(const void* ptr) -> std::string
{
  if (ptr) {
    std::stringstream stream;
    if constexpr (on_msvc) {
      stream << "0x";
    }
    stream << ptr;
    return stream.str();
  }
  else {
    return std::string{};
  }
}
}
namespace cen::detail {
[[nodiscard]] constexpr auto sdl_version_at_least(const int major,
                                                  const int minor,
                                                  const int patch) noexcept -> bool
{
  return SDL_COMPILEDVERSION >= SDL_VERSIONNUM(major, minor, patch);
}
}
namespace cen {
template <typename T>
struct basic_vector3;
using ivec3 = basic_vector3<int>;
using fvec3 = basic_vector3<float>;
template <typename T>
struct basic_vector3 final
{
  static_assert(is_number<T>);
  using value_type = T;
  value_type x{};
  value_type y{};
  value_type z{};
  template <typename U>
  [[nodiscard]] explicit operator basic_vector3<U>() const noexcept
  {
    using target_value_type = typename basic_vector3<U>::value_type;
    return basic_vector3<U>{static_cast<target_value_type>(x),
                            static_cast<target_value_type>(y),
                            static_cast<target_value_type>(z)};
  }
};
template <typename Archive, typename T>
void serialize(Archive& archive, basic_vector3<T>& vector)
{
  archive(vector.x, vector.y, vector.z);
}
template <typename T>
[[nodiscard]] constexpr auto operator==(const basic_vector3<T>& a,
                                        const basic_vector3<T>& b) noexcept -> bool
{
  return a.x == b.x && a.y == b.y && a.z == b.z;
}
template <typename T>
[[nodiscard]] constexpr auto operator!=(const basic_vector3<T>& a,
                                        const basic_vector3<T>& b) noexcept -> bool
{
  return !(a == b);
}
template <typename T>
[[nodiscard]] auto to_string(const basic_vector3<T>& vector) -> std::string
{
  return "(x: " + std::to_string(vector.x) + ", y: " + std::to_string(vector.y) +
         ", z: " + std::to_string(vector.z) + ")";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_vector3<T>& vector) -> std::ostream&
{
  return stream << to_string(vector);
}
template <typename T>
struct basic_area;
using iarea = basic_area<int>;
using farea = basic_area<float>;
template <typename T>
struct basic_area final
{
  static_assert(is_number<T>);
  using value_type = T;
  value_type width{};
  value_type height{};
  template <typename TT = T, std::enable_if_t<std::is_integral_v<TT>, int> = 0>
  [[nodiscard]] constexpr auto as_f() const noexcept -> farea
  {
    return {static_cast<farea::value_type>(width), static_cast<farea::value_type>(height)};
  }
  template <typename TT = T, std::enable_if_t<std::is_floating_point_v<TT>, int> = 0>
  [[nodiscard]] constexpr auto as_i() const noexcept -> iarea
  {
    return {static_cast<iarea::value_type>(width), static_cast<iarea::value_type>(height)};
  }
};
template <typename Archive, typename T>
void serialize(Archive& archive, basic_area<T>& area)
{
  archive(area.width, area.height);
}
template <typename T>
[[nodiscard]] constexpr auto area_of(const basic_area<T> area) noexcept -> T
{
  return area.width * area.height;
}
template <>
[[nodiscard]] constexpr auto cast(const iarea& from) noexcept -> farea
{
  return {static_cast<float>(from.width), static_cast<float>(from.height)};
}
template <>
[[nodiscard]] constexpr auto cast(const farea& from) noexcept -> iarea
{
  return {static_cast<int>(from.width), static_cast<int>(from.height)};
}
template <typename T>
[[nodiscard]] auto to_string(const basic_area<T>& area) -> std::string
{
  return "area(width: " + std::to_string(area.width) +
         ", height: " + std::to_string(area.height) + ")";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_area<T>& area) -> std::ostream&
{
  return stream << to_string(area);
}
template <typename T>
[[nodiscard]] constexpr auto operator==(const basic_area<T>& a,
                                        const basic_area<T>& b) noexcept -> bool
{
  return a.width == b.width && a.height == b.height;
}
template <typename T>
[[nodiscard]] constexpr auto operator!=(const basic_area<T>& a,
                                        const basic_area<T>& b) noexcept -> bool
{
  return !(a == b);
}
template <typename T, enable_for_convertible_t<T, int, float> = 0>
class point_traits final
{
 public:
  inline constexpr static bool integral = std::is_integral_v<T>;
  inline constexpr static bool floating = std::is_floating_point_v<T>;
  using value_type = std::conditional_t<integral, int, float>;
  using point_type = std::conditional_t<integral, SDL_Point, SDL_FPoint>;
};
template <typename T>
class basic_point;
using ipoint = basic_point<int>;
using fpoint = basic_point<float>;
template <typename T>
class basic_point final
{
 public:
  inline constexpr static bool integral = point_traits<T>::integral;
  inline constexpr static bool floating = point_traits<T>::floating;
  using value_type = typename point_traits<T>::value_type;
  using point_type = typename point_traits<T>::point_type;
  constexpr basic_point() noexcept = default;
  constexpr basic_point(const value_type x, const value_type y) noexcept : mPoint{x, y} {}
  constexpr explicit basic_point(const point_type point) noexcept : mPoint{point} {}
  constexpr void set_x(const value_type x) noexcept { mPoint.x = x; }
  constexpr void set_y(const value_type y) noexcept { mPoint.y = y; }
  [[nodiscard]] constexpr auto x() const noexcept -> value_type { return mPoint.x; }
  [[nodiscard]] constexpr auto y() const noexcept -> value_type { return mPoint.y; }
  [[nodiscard]] constexpr auto get() noexcept -> point_type& { return mPoint; }
  [[nodiscard]] constexpr auto get() const noexcept -> const point_type& { return mPoint; }
  template <typename TT = T, std::enable_if_t<std::is_integral_v<TT>, int> = 0>
  [[nodiscard]] constexpr auto as_f() const noexcept -> fpoint
  {
    return {static_cast<fpoint::value_type>(mPoint.x),
            static_cast<fpoint::value_type>(mPoint.y)};
  }
  template <typename TT = T, std::enable_if_t<std::is_floating_point_v<TT>, int> = 0>
  [[nodiscard]] constexpr auto as_i() const noexcept -> ipoint
  {
    return {static_cast<ipoint::value_type>(mPoint.x),
            static_cast<ipoint::value_type>(mPoint.y)};
  }
  template <typename Archive>
  void serialize(Archive& archive)
  {
    archive(mPoint.x, mPoint.y);
  }
  [[nodiscard]] auto data() noexcept -> point_type* { return &mPoint; }
  [[nodiscard]] auto data() const noexcept -> const point_type* { return &mPoint; }
 private:
  point_type mPoint{};
};
template <typename T>
[[nodiscard]] auto distance(const basic_point<T>& from, const basic_point<T>& to) noexcept ->
    typename basic_point<T>::value_type
{
  const auto distance =
      std::sqrt(std::pow(from.x() - to.x(), T{2}) + std::pow(from.y() - to.y(), T{2}));
  if constexpr (basic_point<T>::integral) {
    return static_cast<int>(std::round(distance));
  }
  else {
    return distance;
  }
}
template <>
[[nodiscard]] constexpr auto cast(const fpoint& from) noexcept -> ipoint
{
  const auto x = static_cast<int>(from.x());
  const auto y = static_cast<int>(from.y());
  return ipoint{x, y};
}
template <>
[[nodiscard]] constexpr auto cast(const ipoint& from) noexcept -> fpoint
{
  const auto x = static_cast<float>(from.x());
  const auto y = static_cast<float>(from.y());
  return fpoint{x, y};
}
template <>
[[nodiscard]] constexpr auto cast(const SDL_FPoint& from) noexcept -> SDL_Point
{
  const auto x = static_cast<int>(from.x);
  const auto y = static_cast<int>(from.y);
  return SDL_Point{x, y};
}
template <>
[[nodiscard]] constexpr auto cast(const SDL_Point& from) noexcept -> SDL_FPoint
{
  const auto x = static_cast<float>(from.x);
  const auto y = static_cast<float>(from.y);
  return SDL_FPoint{x, y};
}
template <typename T>
[[nodiscard]] auto to_string(const basic_point<T>& point) -> std::string
{
  return "(x: " + std::to_string(point.x()) + ", y: " + std::to_string(point.y()) + ")";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_point<T>& point) -> std::ostream&
{
  return stream << to_string(point);
}
template <typename T>
[[nodiscard]] constexpr auto operator+(const basic_point<T>& a,
                                       const basic_point<T>& b) noexcept -> basic_point<T>
{
  return {a.x() + b.x(), a.y() + b.y()};
}
template <typename T>
[[nodiscard]] constexpr auto operator-(const basic_point<T>& a,
                                       const basic_point<T>& b) noexcept -> basic_point<T>
{
  return {a.x() - b.x(), a.y() - b.y()};
}
template <typename T>
[[nodiscard]] constexpr auto operator==(const basic_point<T> a,
                                        const basic_point<T> b) noexcept -> bool
{
  return a.x() == b.x() && a.y() == b.y();
}
template <typename T>
[[nodiscard]] constexpr auto operator!=(const basic_point<T> a,
                                        const basic_point<T> b) noexcept -> bool
{
  return !(a == b);
}
template <typename T, enable_for_convertible_t<T, int, float> = 0>
class rect_traits final
{
 public:
  inline constexpr static bool integral = std::is_integral_v<T>;
  inline constexpr static bool floating = std::is_floating_point_v<T>;
  using value_type = std::conditional_t<integral, int, float>;
  using point_type = std::conditional_t<integral, ipoint, fpoint>;
  using area_type = std::conditional_t<integral, iarea, farea>;
  using rect_type = std::conditional_t<integral, SDL_Rect, SDL_FRect>;
};
template <typename T>
class basic_rect;
using irect = basic_rect<int>;
using frect = basic_rect<float>;
template <typename T>
class basic_rect final
{
 public:
  inline constexpr static bool integral = rect_traits<T>::integral;
  inline constexpr static bool floating = rect_traits<T>::floating;
  using value_type = typename rect_traits<T>::value_type;
  using point_type = typename rect_traits<T>::point_type;
  using area_type = typename rect_traits<T>::area_type;
  using rect_type = typename rect_traits<T>::rect_type;
  constexpr basic_rect() noexcept = default;
  constexpr basic_rect(const value_type x,
                       const value_type y,
                       const value_type width,
                       const value_type height) noexcept
      : mRect{x, y, width, height}
  {}
  constexpr basic_rect(const point_type& position, const area_type& size) noexcept
      : mRect{position.x(), position.y(), size.width, size.height}
  {}
  constexpr explicit basic_rect(const rect_type& rect) noexcept : mRect{rect} {}
  constexpr void set_x(const value_type x) noexcept { mRect.x = x; }
  constexpr void set_y(const value_type y) noexcept { mRect.y = y; }
  constexpr void set_position(const value_type x, const value_type y) noexcept
  {
    mRect.x = x;
    mRect.y = y;
  }
  constexpr void set_position(const point_type& pos) noexcept
  {
    set_position(pos.x(), pos.y());
  }
  constexpr void set_width(const value_type width) noexcept { mRect.w = width; }
  constexpr void set_height(const value_type height) noexcept { mRect.h = height; }
  constexpr void set_size(const value_type width, const value_type height) noexcept
  {
    mRect.w = width;
    mRect.h = height;
  }
  constexpr void set_size(const area_type& size) noexcept
  {
    set_size(size.width, size.height);
  }
  constexpr void set_max_x(const value_type mx) noexcept { mRect.x = mx - mRect.w; }
  constexpr void set_max_y(const value_type my) noexcept { mRect.y = my - mRect.h; }
  constexpr void offset_x(const value_type dx) noexcept { mRect.x += dx; }
  constexpr void offset_y(const value_type dy) noexcept { mRect.y += dy; }
  constexpr void offset_width(const value_type dw) noexcept { mRect.w += dw; }
  constexpr void offset_height(const value_type dh) noexcept { mRect.h += dh; }
  [[nodiscard]] constexpr auto x() const noexcept -> value_type { return mRect.x; }
  [[nodiscard]] constexpr auto y() const noexcept -> value_type { return mRect.y; }
  [[nodiscard]] constexpr auto position() const noexcept -> point_type
  {
    return {mRect.x, mRect.y};
  }
  [[nodiscard]] constexpr auto width() const noexcept -> value_type { return mRect.w; }
  [[nodiscard]] constexpr auto height() const noexcept -> value_type { return mRect.h; }
  [[nodiscard]] constexpr auto size() const noexcept -> area_type
  {
    return {mRect.w, mRect.h};
  }
  [[nodiscard]] constexpr auto max_x() const noexcept -> value_type { return x() + width(); }
  [[nodiscard]] constexpr auto max_y() const noexcept -> value_type { return y() + height(); }
  [[nodiscard]] constexpr auto has_area() const noexcept -> bool
  {
    return (width() > 0) && (height() > 0);
  }
  [[nodiscard]] constexpr auto area() const noexcept -> value_type
  {
    return width() * height();
  }
  [[nodiscard]] constexpr auto center_x() const noexcept -> value_type
  {
    return x() + (width() / value_type{2});
  }
  [[nodiscard]] constexpr auto center_y() const noexcept -> value_type
  {
    return y() + (height() / value_type{2});
  }
  [[nodiscard]] constexpr auto center() const noexcept -> point_type
  {
    return {center_x(), center_y()};
  }
  [[nodiscard]] constexpr auto contains(const value_type px,
                                        const value_type py) const noexcept -> bool
  {
    return !(px < x() || py < y() || px > max_x() || py > max_y());
  }
  [[nodiscard]] constexpr auto contains(const point_type& point) const noexcept -> bool
  {
    return contains(point.x(), point.y());
  }
  template <typename TT = T, std::enable_if_t<std::is_integral_v<TT>, int> = 0>
  [[nodiscard]] constexpr auto as_f() const noexcept -> frect
  {
    return {static_cast<frect::value_type>(mRect.x),
            static_cast<frect::value_type>(mRect.y),
            static_cast<frect::value_type>(mRect.w),
            static_cast<frect::value_type>(mRect.h)};
  }
  template <typename TT = T, std::enable_if_t<std::is_floating_point_v<TT>, int> = 0>
  [[nodiscard]] constexpr auto as_i() const noexcept -> irect
  {
    return {static_cast<irect::value_type>(mRect.x),
            static_cast<irect::value_type>(mRect.y),
            static_cast<irect::value_type>(mRect.w),
            static_cast<irect::value_type>(mRect.h)};
  }
  template <typename Archive>
  void serialize(Archive& archive)
  {
    archive(mRect.x, mRect.y, mRect.w, mRect.h);
  }
  [[nodiscard]] auto data() noexcept -> rect_type* { return &mRect; }
  [[nodiscard]] auto data() const noexcept -> const rect_type* { return &mRect; }
  [[nodiscard]] constexpr auto get() noexcept -> rect_type& { return mRect; }
  [[nodiscard]] constexpr auto get() const noexcept -> const rect_type& { return mRect; }
 private:
  rect_type mRect{0, 0, 0, 0};
};
template <typename T>
[[nodiscard]] constexpr auto intersects(const basic_rect<T>& a,
                                        const basic_rect<T>& b) noexcept -> bool
{
  return !(a.x() >= b.max_x() || a.y() >= b.max_y() || a.max_x() <= b.x() ||
           a.max_y() <= b.y());
}
template <typename T>
[[nodiscard]] constexpr auto overlaps(const basic_rect<T>& a, const basic_rect<T>& b) noexcept
    -> bool
{
  return !(a.x() > b.max_x() || a.y() > b.max_y() || a.max_x() < b.x() || a.max_y() < b.y());
}
template <typename T>
[[nodiscard]] constexpr auto get_union(const basic_rect<T>& a, const basic_rect<T>& b) noexcept
    -> basic_rect<T>
{
  if constexpr (detail::sdl_version_at_least(2, 0, 22)) {
    if constexpr (basic_rect<T>::floating) {
      cen::frect res;
      SDL_UnionFRect(a.data(), b.data(), res.data());
      return res;
    }
    else {
      cen::irect res;
      SDL_UnionRect(a.data(), b.data(), res.data());
      return res;
    }
  }
  else {
    const auto aHasArea = a.has_area();
    const auto bHasArea = b.has_area();
    if (!aHasArea && !bHasArea) {
      return {};
    }
    else if (!aHasArea) {
      return b;
    }
    else if (!bHasArea) {
      return a;
    }
    const auto x = detail::min(a.x(), b.x());
    const auto y = detail::min(a.y(), b.y());
    const auto maxX = detail::max(a.max_x(), b.max_x());
    const auto maxY = detail::max(a.max_y(), b.max_y());
    return {{x, y}, {maxX - x, maxY - y}};
  }
}
template <>
[[nodiscard]] constexpr auto cast(const irect& from) noexcept -> frect
{
  const frect::point_type pos{static_cast<float>(from.x()), static_cast<float>(from.y())};
  const frect::area_type size{static_cast<float>(from.width()),
                              static_cast<float>(from.height())};
  return frect{pos, size};
}
template <>
[[nodiscard]] constexpr auto cast(const frect& from) noexcept -> irect
{
  const irect::point_type pos{static_cast<int>(from.x()), static_cast<int>(from.y())};
  const irect::area_type size{static_cast<int>(from.width()), static_cast<int>(from.height())};
  return irect{pos, size};
}
template <typename T>
[[nodiscard]] auto to_string(const basic_rect<T>& rect) -> std::string
{
  return "(x: " + std::to_string(rect.x()) + ", y: " + std::to_string(rect.y()) +
         ", width: " + std::to_string(rect.width()) +
         ", height: " + std::to_string(rect.height()) + ")";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_rect<T>& rect) -> std::ostream&
{
  return stream << to_string(rect);
}
template <typename T>
[[nodiscard]] constexpr auto operator==(const basic_rect<T>& a,
                                        const basic_rect<T>& b) noexcept -> bool
{
  return a.x() == b.x() && a.y() == b.y() && a.width() == b.width() &&
         a.height() == b.height();
}
template <typename T>
[[nodiscard]] constexpr auto operator!=(const basic_rect<T>& a,
                                        const basic_rect<T>& b) noexcept -> bool
{
  return !(a == b);
}
}
namespace cen {
class color final
{
 public:
  constexpr color() noexcept = default;
  constexpr color(const uint8 red,
                  const uint8 green,
                  const uint8 blue,
                  const uint8 alpha = 0xFF) noexcept
      : mColor{red, green, blue, alpha}
  {}
  constexpr explicit color(const SDL_Color& color) noexcept : mColor{color} {}
  constexpr explicit color(const SDL_MessageBoxColor& color) noexcept
      : mColor{color.r, color.g, color.b, 0xFF}
  {}
  [[nodiscard]] static auto from_hsv(float hue, float saturation, float value) -> color
  {
    hue = detail::clamp(hue, 0.0f, 360.0f);
    saturation = detail::clamp(saturation, 0.0f, 100.0f);
    value = detail::clamp(value, 0.0f, 100.0f);
    const auto v = (value / 100.0f);
    const auto chroma = v * (saturation / 100.0f);
    const auto hp = hue / 60.0f;
    const auto x = chroma * (1.0f - std::abs(std::fmod(hp, 2.0f) - 1.0f));
    float red{};
    float green{};
    float blue{};
    if (0 <= hp && hp <= 1) {
      red = chroma;
      green = x;
      blue = 0;
    }
    else if (1 < hp && hp <= 2) {
      red = x;
      green = chroma;
      blue = 0;
    }
    else if (2 < hp && hp <= 3) {
      red = 0;
      green = chroma;
      blue = x;
    }
    else if (3 < hp && hp <= 4) {
      red = 0;
      green = x;
      blue = chroma;
    }
    else if (4 < hp && hp <= 5) {
      red = x;
      green = 0;
      blue = chroma;
    }
    else if (5 < hp && hp <= 6) {
      red = chroma;
      green = 0;
      blue = x;
    }
    const auto m = v - chroma;
    const auto r = static_cast<uint8>(std::round((red + m) * 255.0f));
    const auto g = static_cast<uint8>(std::round((green + m) * 255.0f));
    const auto b = static_cast<uint8>(std::round((blue + m) * 255.0f));
    return {r, g, b};
  }
  [[nodiscard]] static auto from_hsl(float hue, float saturation, float lightness) -> color
  {
    hue = detail::clamp(hue, 0.0f, 360.0f);
    saturation = detail::clamp(saturation, 0.0f, 100.0f);
    lightness = detail::clamp(lightness, 0.0f, 100.0f);
    const auto s = saturation / 100.0f;
    const auto l = lightness / 100.0f;
    const auto chroma = (1.0f - std::abs(2.0f * l - 1.0f)) * s;
    const auto hp = hue / 60.0f;
    const auto x = chroma * (1.0f - std::abs(std::fmod(hp, 2.0f) - 1.0f));
    float red{};
    float green{};
    float blue{};
    if (0 <= hp && hp < 1) {
      red = chroma;
      green = x;
      blue = 0;
    }
    else if (1 <= hp && hp < 2) {
      red = x;
      green = chroma;
      blue = 0;
    }
    else if (2 <= hp && hp < 3) {
      red = 0;
      green = chroma;
      blue = x;
    }
    else if (3 <= hp && hp < 4) {
      red = 0;
      green = x;
      blue = chroma;
    }
    else if (4 <= hp && hp < 5) {
      red = x;
      green = 0;
      blue = chroma;
    }
    else if (5 <= hp && hp < 6) {
      red = chroma;
      green = 0;
      blue = x;
    }
    const auto m = l - (chroma / 2.0f);
    const auto r = static_cast<uint8>(std::round((red + m) * 255.0f));
    const auto g = static_cast<uint8>(std::round((green + m) * 255.0f));
    const auto b = static_cast<uint8>(std::round((blue + m) * 255.0f));
    return {r, g, b};
  }
  [[nodiscard]] static auto from_rgb(const std::string_view rgb) -> maybe<color>
  {
    if (rgb.length() != 7 || rgb.at(0) != '#') {
      return nothing;
    }
    const auto noHash = rgb.substr(1);
    const auto rr = noHash.substr(0, 2);
    const auto gg = noHash.substr(2, 2);
    const auto bb = noHash.substr(4, 2);
    const auto red = detail::stoi<uint8>(rr, 16);
    const auto green = detail::stoi<uint8>(gg, 16);
    const auto blue = detail::stoi<uint8>(bb, 16);
    if (red && green && blue) {
      return color{*red, *green, *blue};
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] static auto from_rgba(const std::string_view rgba) -> maybe<color>
  {
    if (rgba.length() != 9 || rgba.at(0) != '#') {
      return nothing;
    }
    const auto noHash = rgba.substr(1);
    const auto rr = noHash.substr(0, 2);
    const auto gg = noHash.substr(2, 2);
    const auto bb = noHash.substr(4, 2);
    const auto aa = noHash.substr(6, 2);
    const auto red = detail::stoi<uint8>(rr, 16);
    const auto green = detail::stoi<uint8>(gg, 16);
    const auto blue = detail::stoi<uint8>(bb, 16);
    const auto alpha = detail::stoi<uint8>(aa, 16);
    if (red && green && blue && alpha) {
      return color{*red, *green, *blue, *alpha};
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] static auto from_argb(const std::string_view argb) -> maybe<color>
  {
    if (argb.length() != 9 || argb.at(0) != '#') {
      return nothing;
    }
    const auto noHash = argb.substr(1);
    const auto aa = noHash.substr(0, 2);
    const auto rr = noHash.substr(2, 2);
    const auto gg = noHash.substr(4, 2);
    const auto bb = noHash.substr(6, 2);
    const auto alpha = detail::stoi<uint8>(aa, 16);
    const auto red = detail::stoi<uint8>(rr, 16);
    const auto green = detail::stoi<uint8>(gg, 16);
    const auto blue = detail::stoi<uint8>(bb, 16);
    if (alpha && red && green && blue) {
      return color{*red, *green, *blue, *alpha};
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] static auto from_norm(float red,
                                      float green,
                                      float blue,
                                      float alpha = 1.0f) noexcept(on_msvc) -> color
  {
    red = detail::clamp(red, 0.0f, 1.0f);
    green = detail::clamp(green, 0.0f, 1.0f);
    blue = detail::clamp(blue, 0.0f, 1.0f);
    alpha = detail::clamp(alpha, 0.0f, 1.0f);
    const auto r = static_cast<uint8>(std::round(red * 255.0f));
    const auto g = static_cast<uint8>(std::round(green * 255.0f));
    const auto b = static_cast<uint8>(std::round(blue * 255.0f));
    const auto a = static_cast<uint8>(std::round(alpha * 255.0f));
    return color{r, g, b, a};
  }
  [[nodiscard]] constexpr auto with_alpha(const uint8 alpha) const noexcept -> color
  {
    return {red(), green(), blue(), alpha};
  }
  constexpr void set_red(const uint8 red) noexcept { mColor.r = red; }
  constexpr void set_green(const uint8 green) noexcept { mColor.g = green; }
  constexpr void set_blue(const uint8 blue) noexcept { mColor.b = blue; }
  constexpr void set_alpha(const uint8 alpha) noexcept { mColor.a = alpha; }
  [[nodiscard]] constexpr auto red() const noexcept -> uint8 { return mColor.r; }
  [[nodiscard]] constexpr auto green() const noexcept -> uint8 { return mColor.g; }
  [[nodiscard]] constexpr auto blue() const noexcept -> uint8 { return mColor.b; }
  [[nodiscard]] constexpr auto alpha() const noexcept -> uint8 { return mColor.a; }
  [[nodiscard]] constexpr auto norm_red() const noexcept -> float
  {
    return static_cast<float>(mColor.r) / 255.0f;
  }
  [[nodiscard]] constexpr auto norm_green() const noexcept -> float
  {
    return static_cast<float>(mColor.g) / 255.0f;
  }
  [[nodiscard]] constexpr auto norm_blue() const noexcept -> float
  {
    return static_cast<float>(mColor.b) / 255.0f;
  }
  [[nodiscard]] constexpr auto norm_alpha() const noexcept -> float
  {
    return static_cast<float>(mColor.a) / 255.0f;
  }
  [[nodiscard]] auto as_rgb() const -> std::string
  {
    std::stringstream stream;
    stream << std::setfill('0') << std::hex << std::uppercase;
    stream << '#';
    stream << std::setw(2) << +mColor.r;
    stream << std::setw(2) << +mColor.g;
    stream << std::setw(2) << +mColor.b;
    return stream.str();
  }
  [[nodiscard]] auto as_rgba() const -> std::string
  {
    std::stringstream stream;
    stream << std::setfill('0') << std::hex << std::uppercase;
    stream << '#';
    stream << std::setw(2) << +mColor.r;
    stream << std::setw(2) << +mColor.g;
    stream << std::setw(2) << +mColor.b;
    stream << std::setw(2) << +mColor.a;
    return stream.str();
  }
  [[nodiscard]] auto as_argb() const -> std::string
  {
    std::stringstream stream;
    stream << std::setfill('0') << std::hex << std::uppercase;
    stream << '#';
    stream << std::setw(2) << +mColor.a;
    stream << std::setw(2) << +mColor.r;
    stream << std::setw(2) << +mColor.g;
    stream << std::setw(2) << +mColor.b;
    return stream.str();
  }
  [[nodiscard]] auto as_message_box_color() const noexcept -> SDL_MessageBoxColor
  {
    return {mColor.r, mColor.g, mColor.b};
  }
  template <typename Archive>
  void serialize(Archive& archive)
  {
    archive(mColor.r, mColor.g, mColor.b, mColor.a);
  }
  [[nodiscard]] auto data() noexcept -> SDL_Color* { return &mColor; }
  [[nodiscard]] auto data() const noexcept -> const SDL_Color* { return &mColor; }
  [[nodiscard]] auto get() const noexcept -> const SDL_Color& { return mColor; }
 private:
  SDL_Color mColor{0, 0, 0, 0xFF};
};
[[nodiscard]] inline auto blend(const color& a, const color& b, const float bias = 0.5f)
    -> color
{
  assert(bias >= 0);
  assert(bias <= 1.0f);
  const auto red = detail::lerp(a.norm_red(), b.norm_red(), bias);
  const auto green = detail::lerp(a.norm_green(), b.norm_green(), bias);
  const auto blue = detail::lerp(a.norm_blue(), b.norm_blue(), bias);
  const auto alpha = detail::lerp(a.norm_alpha(), b.norm_alpha(), bias);
  return color::from_norm(red, green, blue, alpha);
}
[[nodiscard]] inline auto to_string(const color& color) -> std::string
{
  return color.as_rgba();
}
inline auto operator<<(std::ostream& stream, const color& color) -> std::ostream&
{
  return stream << to_string(color);
}
[[nodiscard]] constexpr auto operator==(const color& a, const color& b) noexcept -> bool
{
  return (a.red() == b.red()) && (a.green() == b.green()) && (a.blue() == b.blue()) &&
         (a.alpha() == b.alpha());
}
[[nodiscard]] constexpr auto operator!=(const color& a, const color& b) noexcept -> bool
{
  return !(a == b);
}
namespace colors {
inline constexpr color transparent{0, 0, 0, 0};
inline constexpr color white{0xFF, 0xFF, 0xFF};
inline constexpr color black{0, 0, 0};
inline constexpr color alice_blue{0xF0, 0xF8, 0xFF};
inline constexpr color antique_white{0xFA, 0xEB, 0xD7};
inline constexpr color aqua{0, 0xFF, 0xFF};
inline constexpr color aquamarine{0x7F, 0xFF, 0xD4};
inline constexpr color azure{0xF0, 0xFF, 0xFF};
inline constexpr color beige{0xF5, 0xF5, 0xDC};
inline constexpr color bisque{0xFF, 0xE4, 0xC4};
inline constexpr color blanched_almond{0xFF, 0xEB, 0xCD};
inline constexpr color blue{0, 0, 0xFF};
inline constexpr color blue_violet{0x8A, 0x2B, 0xE2};
inline constexpr color brown{0xA5, 0x2A, 0x2A};
inline constexpr color burly_wood{0xDE, 0xB8, 0x87};
inline constexpr color cadet_blue{0x5F, 0x9E, 0xA0};
inline constexpr color chartreuse{0x7F, 0xFF, 0};
inline constexpr color chocolate{0xD2, 0x69, 0x1E};
inline constexpr color coral{0xFF, 0x7F, 0x50};
inline constexpr color cornflower_blue{0x64, 0x95, 0xED};
inline constexpr color cornsilk{0xFF, 0xF8, 0xDC};
inline constexpr color crimson{0xDC, 0x14, 0x3C};
inline constexpr color cyan{0, 0xFF, 0xFF};
inline constexpr color deep_pink{0xFF, 0x14, 0x93};
inline constexpr color deep_sky_blue{0, 0xBF, 0xFF};
inline constexpr color dim_gray{0x69, 0x69, 0x69};
inline constexpr color dim_grey{dim_gray};
inline constexpr color dodger_blue{0x1E, 0x90, 0xFF};
inline constexpr color fire_brick{0xB2, 0x22, 0x22};
inline constexpr color floral_white{0xFF, 0xFA, 0xF0};
inline constexpr color forest_green{0x22, 0x8B, 0x22};
inline constexpr color fuchsia{0xFF, 0, 0xFF};
inline constexpr color gainsboro{0xDC, 0xDC, 0xDC};
inline constexpr color ghost_white{0xF8, 0xF8, 0xFF};
inline constexpr color gold{0xFF, 0xD7, 0};
inline constexpr color golden_rod{0xDA, 0xA5, 0x20};
inline constexpr color gray{0x80, 0x80, 0x80};
inline constexpr color grey{gray};
inline constexpr color green{0, 0x80, 0};
inline constexpr color green_yellow{0xAD, 0xFF, 0x2F};
inline constexpr color honey_dew{0xF0, 0xFF, 0xF0};
inline constexpr color hot_pink{0xFF, 0x69, 0xB4};
inline constexpr color indian_red{0xCD, 0x5C, 0x5C};
inline constexpr color indigo{0x4B, 0, 0x82};
inline constexpr color ivory{0xFF, 0xFF, 0xF0};
inline constexpr color khaki{0xF0, 0xE6, 0x8C};
inline constexpr color lavender{0xE6, 0xE6, 0xFA};
inline constexpr color lavender_blush{0xFF, 0xF0, 0xF5};
inline constexpr color lawn_green{0x7C, 0xFC, 0};
inline constexpr color lemon_chiffon{0xFF, 0xFA, 0xCD};
inline constexpr color lime{0, 0xFF, 0};
inline constexpr color lime_green{0x32, 0xCD, 0x32};
inline constexpr color linen{0xFA, 0xF0, 0xE6};
inline constexpr color magenta{0xFF, 0, 0xFF};
inline constexpr color maroon{0x80, 0, 0};
inline constexpr color midnight_blue{0x19, 0x19, 0x70};
inline constexpr color mint_cream{0xF5, 0xFF, 0xFA};
inline constexpr color misty_rose{0xFF, 0xE4, 0xE1};
inline constexpr color moccasin{0xFF, 0xE4, 0xB5};
inline constexpr color navajo_white{0xFF, 0xDE, 0xAD};
inline constexpr color navy{0, 0, 0x80};
inline constexpr color old_lace{0xFD, 0xF5, 0xE6};
inline constexpr color olive{0x80, 0x80, 0};
inline constexpr color olive_drab{0x6B, 0x8E, 0x23};
inline constexpr color orange{0xFF, 0xA5, 0};
inline constexpr color orange_red{0xFF, 0x45, 0};
inline constexpr color orchid{0xDA, 0x70, 0xD6};
inline constexpr color pale_golden_rod{0xEE, 0xE8, 0xAA};
inline constexpr color pale_green{0x98, 0xFB, 0x98};
inline constexpr color pale_turquoise{0xAF, 0xEE, 0xEE};
inline constexpr color pale_violet_red{0xDB, 0x70, 0x93};
inline constexpr color papaya_whip{0xFF, 0xEF, 0xD5};
inline constexpr color peach_puff{0xFF, 0xDA, 0xB9};
inline constexpr color peru{0xCD, 0x85, 0x3F};
inline constexpr color pink{0xFF, 0xC0, 0xCB};
inline constexpr color plum{0xDD, 0xA0, 0xDD};
inline constexpr color powder_blue{0xB0, 0xE0, 0xE6};
inline constexpr color purple{0x80, 0, 0x80};
inline constexpr color rebecca_purple{0x66, 0x33, 0x99};
inline constexpr color red{0xFF, 0, 0};
inline constexpr color rosy_brown{0xBC, 0x8F, 0x8F};
inline constexpr color royal_blue{0x41, 0x69, 0xE1};
inline constexpr color saddle_brown{0x8B, 0x45, 0x13};
inline constexpr color salmon{0xFA, 0x80, 0x72};
inline constexpr color sandy_brown{0xF4, 0xA4, 0x60};
inline constexpr color sea_green{0x2E, 0x8B, 0x57};
inline constexpr color sea_shell{0xFF, 0xF5, 0xEE};
inline constexpr color sienna{0xA0, 0x52, 0x2D};
inline constexpr color silver{0xC0, 0xC0, 0xC0};
inline constexpr color sky_blue{0x87, 0xCE, 0xEB};
inline constexpr color slate_blue{0x6A, 0x5A, 0xCD};
inline constexpr color slate_gray{0x70, 0x80, 0x90};
inline constexpr color slate_grey{slate_gray};
inline constexpr color snow{0xFF, 0xFA, 0xFA};
inline constexpr color spring_green{0, 0xFF, 0x7F};
inline constexpr color steel_blue{0x46, 0x82, 0xB4};
inline constexpr color tan{0xD2, 0xB4, 0x8C};
inline constexpr color teal{0, 0x80, 0x80};
inline constexpr color thistle{0xD8, 0xBF, 0xD8};
inline constexpr color tomato{0xFF, 0x63, 0x47};
inline constexpr color turquoise{0x40, 0xE0, 0xD0};
inline constexpr color violet{0xEE, 0x82, 0xEE};
inline constexpr color wheat{0xF5, 0xDE, 0xB3};
inline constexpr color white_smoke{0xF5, 0xF5, 0xF5};
inline constexpr color yellow{0xFF, 0xFF, 0};
inline constexpr color yellow_green{0x9A, 0xCD, 0x32};
inline constexpr color light_blue{0xAD, 0xD8, 0xE6};
inline constexpr color light_coral{0xF0, 0x80, 0x80};
inline constexpr color light_cyan{0xE0, 0xFF, 0xFF};
inline constexpr color light_golden_rod_yellow{0xFA, 0xFA, 0xD2};
inline constexpr color light_gray{0xD3, 0xD3, 0xD3};
inline constexpr color light_grey{light_gray};
inline constexpr color light_green{0x90, 0xEE, 0x90};
inline constexpr color light_pink{0xFF, 0xB6, 0xC1};
inline constexpr color light_salmon{0xFF, 0xA0, 0x7A};
inline constexpr color light_sea_green{0x20, 0xB2, 0xAA};
inline constexpr color light_sky_blue{0x87, 0xCE, 0xFA};
inline constexpr color light_slate_gray{0x77, 0x88, 0x99};
inline constexpr color light_slate_grey{light_slate_gray};
inline constexpr color light_steel_blue{0xB0, 0xC4, 0xDE};
inline constexpr color light_yellow{0xFF, 0xFF, 0xE0};
inline constexpr color medium_aqua_marine{0x66, 0xCD, 0xAA};
inline constexpr color medium_blue{0, 0, 0xCD};
inline constexpr color medium_orchid{0xBA, 0x55, 0xD3};
inline constexpr color medium_purple{0x93, 0x70, 0xDB};
inline constexpr color medium_sea_green{0x3C, 0xB3, 0x71};
inline constexpr color medium_slate_blue{0x7B, 0x68, 0xEE};
inline constexpr color medium_spring_green{0, 0xFA, 0x9A};
inline constexpr color medium_turquoise{0x48, 0xD1, 0xCC};
inline constexpr color medium_violet_red{0xC7, 0x15, 0x85};
inline constexpr color dark_blue{0, 0, 0x8B};
inline constexpr color dark_cyan{0, 0x8B, 0x8B};
inline constexpr color dark_golden_rod{0xB8, 0x86, 0x0B};
inline constexpr color dark_gray{0xA9, 0xA9, 0xA9};
inline constexpr color dark_grey{dark_gray};
inline constexpr color dark_green{0, 0x64, 0};
inline constexpr color dark_khaki{0xBD, 0xB7, 0x6B};
inline constexpr color dark_magenta{0x8B, 0, 0x8B};
inline constexpr color dark_olive_green{0x55, 0x6B, 0x2F};
inline constexpr color dark_orange{0xFF, 0x8C, 0};
inline constexpr color dark_orchid{0x99, 0x32, 0xCC};
inline constexpr color dark_red{0x8B, 0, 0};
inline constexpr color dark_salmon{0xE9, 0x96, 0x7A};
inline constexpr color dark_sea_green{0x8F, 0xBC, 0x8F};
inline constexpr color dark_slate_blue{0x48, 0x3D, 0x8B};
inline constexpr color dark_slate_gray{0x2F, 0x4F, 0x4F};
inline constexpr color dark_slate_grey{dark_slate_gray};
inline constexpr color dark_turquoise{0, 0xCE, 0xD1};
inline constexpr color dark_violet{0x94, 0, 0xD3};
}
}
namespace cen::detail {
using owner_tag = std::true_type;
using handle_tag = std::false_type;
template <typename T>
inline constexpr bool is_owner = std::is_same_v<T, owner_tag>;
template <typename T>
inline constexpr bool is_handle = std::is_same_v<T, handle_tag>;
template <typename T>
using enable_for_owner = std::enable_if_t<is_owner<T>, int>;
template <typename T>
using enable_for_handle = std::enable_if_t<is_handle<T>, int>;
template <typename B, typename Type>
class pointer final
{
 public:
  using smart_ptr = managed_ptr<Type>;
  using raw_ptr = Type*;
  using pointer_type = std::conditional_t<B::value, smart_ptr, raw_ptr>;
  pointer() noexcept = default;
  explicit pointer(Type* ptr) noexcept : mPtr{ptr} {}
  template <typename BB = B, enable_for_owner<BB> = 0>
  void reset(Type* ptr) noexcept
  {
    mPtr.reset(ptr);
  }
  auto operator->() noexcept -> Type* { return get(); }
  auto operator->() const noexcept -> const Type* { return get(); }
  auto operator*() noexcept -> Type&
  {
    assert(mPtr);
    return *mPtr;
  }
  auto operator*() const noexcept -> const Type&
  {
    assert(mPtr);
    return *mPtr;
  }
  explicit operator bool() const noexcept { return mPtr != nullptr; }
               operator Type*() const noexcept { return get(); }
  template <typename BB = B, enable_for_owner<BB> = 0>
  [[nodiscard]] auto release() noexcept -> Type*
  {
    return mPtr.release();
  }
  [[nodiscard]] auto get() const noexcept -> Type*
  {
    if constexpr (B::value) {
      return mPtr.get();
    }
    else {
      return mPtr;
    }
  }
 private:
  pointer_type mPtr{};
};
}
namespace cen {
enum class pixel_format : uint32
{
  unknown = SDL_PIXELFORMAT_UNKNOWN,
  index1_lsb = SDL_PIXELFORMAT_INDEX1LSB,
  index1_msb = SDL_PIXELFORMAT_INDEX1MSB,
  index4_lsb = SDL_PIXELFORMAT_INDEX4LSB,
  index4_msb = SDL_PIXELFORMAT_INDEX4MSB,
  index8 = SDL_PIXELFORMAT_INDEX8,
  rgba32 = SDL_PIXELFORMAT_RGBA32,
  argb32 = SDL_PIXELFORMAT_ARGB32,
  bgra32 = SDL_PIXELFORMAT_BGRA32,
  abgr32 = SDL_PIXELFORMAT_ABGR32,
  rgb332 = SDL_PIXELFORMAT_RGB332,
  rgb444 = SDL_PIXELFORMAT_RGB444,
  rgb555 = SDL_PIXELFORMAT_RGB555,
  bgr555 = SDL_PIXELFORMAT_BGR555,
  argb4444 = SDL_PIXELFORMAT_ARGB4444,
  rgba4444 = SDL_PIXELFORMAT_RGBA4444,
  abgr4444 = SDL_PIXELFORMAT_ABGR4444,
  bgra4444 = SDL_PIXELFORMAT_BGRA4444,
  argb1555 = SDL_PIXELFORMAT_ARGB1555,
  rgba5551 = SDL_PIXELFORMAT_RGBA5551,
  abgr1555 = SDL_PIXELFORMAT_ABGR1555,
  bgra5551 = SDL_PIXELFORMAT_BGRA5551,
  rgb565 = SDL_PIXELFORMAT_RGB565,
  bgr565 = SDL_PIXELFORMAT_BGR565,
  rgb24 = SDL_PIXELFORMAT_RGB24,
  bgr24 = SDL_PIXELFORMAT_BGR24,
  rgb888 = SDL_PIXELFORMAT_RGB888,
  bgr888 = SDL_PIXELFORMAT_BGR888,
  rgbx8888 = SDL_PIXELFORMAT_RGBX8888,
  bgrx8888 = SDL_PIXELFORMAT_BGRX8888,
  argb8888 = SDL_PIXELFORMAT_ARGB8888,
  rgba8888 = SDL_PIXELFORMAT_RGBA8888,
  abgr8888 = SDL_PIXELFORMAT_ABGR8888,
  bgra8888 = SDL_PIXELFORMAT_BGRA8888,
  argb2101010 = SDL_PIXELFORMAT_ARGB2101010,
  yv12 = SDL_PIXELFORMAT_YV12,
  iyuv = SDL_PIXELFORMAT_IYUV,
  yuy2 = SDL_PIXELFORMAT_YUY2,
  uyvy = SDL_PIXELFORMAT_UYVY,
  yvyu = SDL_PIXELFORMAT_YVYU,
  nv12 = SDL_PIXELFORMAT_NV12,
  nv21 = SDL_PIXELFORMAT_NV21,
  external_oes = SDL_PIXELFORMAT_EXTERNAL_OES,
};
[[nodiscard]] constexpr auto to_string(const pixel_format format) -> std::string_view
{
  switch (format) {
    case pixel_format::unknown:
      return "unknown";
    case pixel_format::index1_lsb:
      return "index1_lsb";
    case pixel_format::index1_msb:
      return "index1_msb";
    case pixel_format::index4_lsb:
      return "index4_lsb";
    case pixel_format::index4_msb:
      return "index4_msb";
    case pixel_format::index8:
      return "index8";
    case pixel_format::rgb332:
      return "rgb332";
    case pixel_format::argb4444:
      return "argb4444";
    case pixel_format::rgba4444:
      return "rgba4444";
    case pixel_format::abgr4444:
      return "abgr4444";
    case pixel_format::bgra4444:
      return "bgra4444";
    case pixel_format::argb1555:
      return "argb1555";
    case pixel_format::rgba5551:
      return "rgba5551";
    case pixel_format::abgr1555:
      return "abgr1555";
    case pixel_format::bgra5551:
      return "bgra5551";
    case pixel_format::rgb565:
      return "rgb565";
    case pixel_format::bgr565:
      return "bgr565";
    case pixel_format::rgb24:
      return "rgb24";
    case pixel_format::bgr24:
      return "bgr24";
    case pixel_format::rgbx8888:
      return "rgbx8888";
    case pixel_format::bgrx8888:
      return "bgrx8888";
    case pixel_format::argb8888:
      return "argb8888";
    case pixel_format::rgba8888:
      return "rgba8888";
    case pixel_format::abgr8888:
      return "abgr8888";
    case pixel_format::bgra8888:
      return "bgra8888";
    case pixel_format::argb2101010:
      return "argb2101010";
    case pixel_format::yv12:
      return "yv12";
    case pixel_format::iyuv:
      return "iyuv";
    case pixel_format::yuy2:
      return "yuy2";
    case pixel_format::uyvy:
      return "uyvy";
    case pixel_format::yvyu:
      return "yvyu";
    case pixel_format::nv12:
      return "nv12";
    case pixel_format::nv21:
      return "nv21";
    case pixel_format::external_oes:
      return "external_oes";
    default:
      throw exception{"Did not recognize pixel format!"};
  }
}
inline auto operator<<(std::ostream& stream, const pixel_format format) -> std::ostream&
{
  return stream << to_string(format);
}
class palette final
{
 public:
  using iterator = SDL_Color*;
  using const_iterator = const SDL_Color*;
  explicit palette(const int count) : mPalette{SDL_AllocPalette(count)}
  {
    if (!mPalette) {
      throw sdl_error{};
    }
  }
  auto set_color(const int index, const color& color) noexcept -> result
  {
    assert(index >= 0);
    assert(index < size());
    return SDL_SetPaletteColors(mPalette.get(), color.data(), index, 1) == 0;
  }
  [[nodiscard]] auto at(const int index) const -> color
  {
    if (index >= 0 && index < size()) {
      return color{mPalette->colors[index]};
    }
    else {
      throw exception{"Palette index out of bounds!"};
    }
  }
  [[nodiscard]] auto operator[](const int index) const -> color { return at(index); }
  [[nodiscard]] auto size() const noexcept -> int { return mPalette->ncolors; }
  [[nodiscard]] auto version() const noexcept -> uint32 { return mPalette->version; }
  [[nodiscard]] auto get() const noexcept -> SDL_Palette* { return mPalette.get(); }
  [[nodiscard]] auto begin() noexcept -> iterator { return mPalette->colors; }
  [[nodiscard]] auto begin() const noexcept -> const_iterator { return mPalette->colors; }
  [[nodiscard]] auto end() noexcept -> iterator { return mPalette->colors + size(); }
  [[nodiscard]] auto end() const noexcept -> const_iterator
  {
    return mPalette->colors + size();
  }
 private:
  managed_ptr<SDL_Palette> mPalette;
};
[[nodiscard]] inline auto to_string(const palette& palette) -> std::string
{
  return "palette(data: " + detail::address_of(palette.get()) +
         ", size: " + std::to_string(palette.size()) + ")";
}
inline auto operator<<(std::ostream& stream, const palette& palette) -> std::ostream&
{
  return stream << to_string(palette);
}
template <typename T>
class basic_pixel_format_info;
using pixel_format_info = basic_pixel_format_info<detail::owner_tag>;
using pixel_format_info_handle = basic_pixel_format_info<detail::handle_tag>;
template <typename T>
class basic_pixel_format_info final
{
 public:
  explicit basic_pixel_format_info(maybe_owner<SDL_PixelFormat*> format) noexcept(detail::is_handle<T>)
      : mFormat{format}
  {
    if constexpr (detail::is_owner<T>) {
      if (!mFormat) {
        throw exception{"Null pixel format!"};
      }
    }
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit basic_pixel_format_info(const pixel_format format)
      : mFormat{SDL_AllocFormat(to_underlying(format))}
  {
    if (!mFormat) {
      throw sdl_error{};
    }
  }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit basic_pixel_format_info(const pixel_format_info& owner) noexcept
      : mFormat{owner.get()}
  {}
  [[nodiscard]] auto pixel_to_rgb(const uint32 pixel) const noexcept -> color
  {
    uint8 red{};
    uint8 green{};
    uint8 blue{};
    SDL_GetRGB(pixel, mFormat, &red, &green, &blue);
    return {red, green, blue};
  }
  [[nodiscard]] auto pixel_to_rgba(const uint32 pixel) const noexcept -> color
  {
    uint8 red{};
    uint8 green{};
    uint8 blue{};
    uint8 alpha{};
    SDL_GetRGBA(pixel, mFormat, &red, &green, &blue, &alpha);
    return {red, green, blue, alpha};
  }
  [[nodiscard]] auto rgb_to_pixel(const color& color) const noexcept -> uint32
  {
    return SDL_MapRGB(mFormat, color.red(), color.green(), color.blue());
  }
  [[nodiscard]] auto rgba_to_pixel(const color& color) const noexcept -> uint32
  {
    return SDL_MapRGBA(mFormat, color.red(), color.green(), color.blue(), color.alpha());
  }
  [[nodiscard]] auto format() const noexcept -> pixel_format
  {
    return static_cast<pixel_format>(mFormat->format);
  }
  [[nodiscard]] auto name() const noexcept -> const char*
  {
    return SDL_GetPixelFormatName(mFormat->format);
  }
  [[nodiscard]] auto get() const noexcept -> SDL_PixelFormat* { return mFormat.get(); }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  [[nodiscard]] explicit operator bool() const noexcept
  {
    return mFormat;
  }
 private:
  detail::pointer<T, SDL_PixelFormat> mFormat;
};
template <typename T>
[[nodiscard]] auto to_string(const basic_pixel_format_info<T>& info) -> std::string
{
  return "pixel_format_info(data: " + detail::address_of(info.get()) + ", name: '" +
         info.name() + "')";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_pixel_format_info<T>& info) -> std::ostream&
{
  return stream << to_string(info);
}
}
namespace cen {
enum class blend_mode
{
  none = SDL_BLENDMODE_NONE,
  blend = SDL_BLENDMODE_BLEND,
  add = SDL_BLENDMODE_ADD,
  mod = SDL_BLENDMODE_MOD,
  invalid = SDL_BLENDMODE_INVALID
};
[[nodiscard]] constexpr auto to_string(const blend_mode mode) -> std::string_view
{
  switch (mode) {
    case blend_mode::none:
      return "none";
    case blend_mode::blend:
      return "blend";
    case blend_mode::add:
      return "add";
    case blend_mode::mod:
      return "mod";
    case blend_mode::invalid:
      return "invalid";
    default:
      throw exception{"Did not recognize blend mode!"};
  }
}
inline auto operator<<(std::ostream& stream, const blend_mode mode) -> std::ostream&
{
  return stream << to_string(mode);
}
enum class blend_factor
{
  zero = SDL_BLENDFACTOR_ZERO,
  one = SDL_BLENDFACTOR_ONE,
  src_color = SDL_BLENDFACTOR_SRC_COLOR,
  one_minus_src_color = SDL_BLENDFACTOR_ONE_MINUS_SRC_COLOR,
  src_alpha = SDL_BLENDFACTOR_SRC_ALPHA,
  one_minus_src_alpha = SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
  dst_color = SDL_BLENDFACTOR_DST_COLOR,
  one_minus_dst_color = SDL_BLENDFACTOR_ONE_MINUS_DST_COLOR,
  dst_alpha = SDL_BLENDFACTOR_DST_ALPHA,
  one_minus_dst_alpha = SDL_BLENDFACTOR_ONE_MINUS_DST_ALPHA
};
[[nodiscard]] constexpr auto to_string(const blend_factor factor) -> std::string_view
{
  switch (factor) {
    case blend_factor::zero:
      return "zero";
    case blend_factor::one:
      return "one";
    case blend_factor::src_color:
      return "src_color";
    case blend_factor::one_minus_src_color:
      return "one_minus_src_color";
    case blend_factor::src_alpha:
      return "src_alpha";
    case blend_factor::one_minus_src_alpha:
      return "one_minus_src_alpha";
    case blend_factor::dst_color:
      return "dst_color";
    case blend_factor::one_minus_dst_color:
      return "one_minus_dst_color";
    case blend_factor::dst_alpha:
      return "dst_alpha";
    case blend_factor::one_minus_dst_alpha:
      return "one_minus_dst_alpha";
    default:
      throw exception{"Did not recognize blend factor!"};
  }
}
inline auto operator<<(std::ostream& stream, const blend_factor factor) -> std::ostream&
{
  return stream << to_string(factor);
}
enum class blend_op
{
  add = SDL_BLENDOPERATION_ADD,
  sub = SDL_BLENDOPERATION_SUBTRACT,
  reverse_sub = SDL_BLENDOPERATION_REV_SUBTRACT,
  min = SDL_BLENDOPERATION_MINIMUM,
  max = SDL_BLENDOPERATION_MAXIMUM
};
[[nodiscard]] constexpr auto to_string(const blend_op op) -> std::string_view
{
  switch (op) {
    case blend_op::add:
      return "add";
    case blend_op::sub:
      return "sub";
    case blend_op::reverse_sub:
      return "reverse_sub";
    case blend_op::min:
      return "min";
    case blend_op::max:
      return "max";
    default:
      throw exception{"Did not recognize blend operation!"};
  }
}
inline auto operator<<(std::ostream& stream, const blend_op op) -> std::ostream&
{
  return stream << to_string(op);
}
enum class orientation
{
  unknown = SDL_ORIENTATION_UNKNOWN,
  landscape = SDL_ORIENTATION_LANDSCAPE,
  landscape_flipped = SDL_ORIENTATION_LANDSCAPE_FLIPPED,
  portrait = SDL_ORIENTATION_PORTRAIT,
  portrait_flipped = SDL_ORIENTATION_PORTRAIT_FLIPPED
};
[[nodiscard]] constexpr auto to_string(const orientation o) -> std::string_view
{
  switch (o) {
    case orientation::unknown:
      return "unknown";
    case orientation::landscape:
      return "landscape";
    case orientation::landscape_flipped:
      return "landscape_flipped";
    case orientation::portrait:
      return "portrait";
    case orientation::portrait_flipped:
      return "portrait_flipped";
    default:
      throw exception{"Did not recognize display orientation!"};
  }
}
inline auto operator<<(std::ostream& stream, const orientation o) -> std::ostream&
{
  return stream << to_string(o);
}
class display_mode final
{
 public:
  [[nodiscard]] static auto current(const int index = 0) -> display_mode
  {
    SDL_DisplayMode mode{};
    if (SDL_GetCurrentDisplayMode(index, &mode) == 0) {
      return display_mode{mode};
    }
    else {
      throw sdl_error{};
    }
  }
  [[nodiscard]] static auto desktop(const int index = 0) -> display_mode
  {
    SDL_DisplayMode mode{};
    if (SDL_GetDesktopDisplayMode(index, &mode) == 0) {
      return display_mode{mode};
    }
    else {
      throw sdl_error{};
    }
  }
  [[nodiscard]] auto format() const noexcept -> pixel_format
  {
    return static_cast<pixel_format>(mMode.format);
  }
  [[nodiscard]] auto width() const noexcept -> int { return mMode.w; }
  [[nodiscard]] auto height() const noexcept -> int { return mMode.h; }
  [[nodiscard]] auto size() const noexcept -> iarea { return {mMode.w, mMode.h}; }
  [[nodiscard]] auto refresh_rate() const noexcept -> maybe<int>
  {
    if (mMode.refresh_rate != 0) {
      return mMode.refresh_rate;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto driver_data() noexcept -> void* { return mMode.driverdata; }
  [[nodiscard]] auto driver_data() const noexcept -> const void* { return mMode.driverdata; }
 private:
  SDL_DisplayMode mMode{};
  explicit display_mode(const SDL_DisplayMode mode) : mMode{mode} {}
};
struct blend_task final
{
  blend_factor src;
  blend_factor dst;
  blend_op op;
};
struct dpi_info final
{
  float diagonal{};
  float horizontal{};
  float vertical{};
};
[[nodiscard]] inline auto compose_blend_mode(const blend_task& color,
                                             const blend_task& alpha) noexcept -> blend_mode
{
  const auto res = SDL_ComposeCustomBlendMode(static_cast<SDL_BlendFactor>(color.src),
                                              static_cast<SDL_BlendFactor>(color.dst),
                                              static_cast<SDL_BlendOperation>(color.op),
                                              static_cast<SDL_BlendFactor>(alpha.src),
                                              static_cast<SDL_BlendFactor>(alpha.dst),
                                              static_cast<SDL_BlendOperation>(alpha.op));
  return static_cast<blend_mode>(res);
}
inline void set_screen_saver_enabled(const bool enabled) noexcept
{
  if (enabled) {
    SDL_EnableScreenSaver();
  }
  else {
    SDL_DisableScreenSaver();
  }
}
[[nodiscard]] inline auto is_screen_saver_enabled() noexcept -> bool
{
  return SDL_IsScreenSaverEnabled();
}
[[nodiscard]] inline auto display_count() noexcept -> maybe<int>
{
  const auto count = SDL_GetNumVideoDisplays();
  if (count >= 1) {
    return count;
  }
  else {
    return nothing;
  }
}
[[nodiscard]] inline auto display_name(const int index = 0) -> maybe<std::string>
{
  if (const char* name = SDL_GetDisplayName(index)) {
    return std::string{name};
  }
  else {
    return nothing;
  }
}
[[nodiscard]] inline auto display_orientation(const int index = 0) noexcept -> orientation
{
  return static_cast<orientation>(SDL_GetDisplayOrientation(index));
}
[[nodiscard]] inline auto display_dpi(const int index = 0) noexcept -> maybe<dpi_info>
{
  dpi_info info;
  if (SDL_GetDisplayDPI(index, &info.diagonal, &info.horizontal, &info.vertical) == 0) {
    return info;
  }
  else {
    return nothing;
  }
}
[[nodiscard]] inline auto display_bounds(const int index = 0) noexcept -> maybe<irect>
{
  irect result;
  if (SDL_GetDisplayBounds(index, result.data()) == 0) {
    return result;
  }
  else {
    return nothing;
  }
}
[[nodiscard]] inline auto display_usable_bounds(const int index = 0) noexcept -> maybe<irect>
{
  irect result;
  if (SDL_GetDisplayUsableBounds(index, result.data()) == 0) {
    return result;
  }
  else {
    return nothing;
  }
}
}
namespace cen {
template <typename T>
class basic_surface;
using surface = basic_surface<detail::owner_tag>;
using surface_handle = basic_surface<detail::handle_tag>;
template <typename T>
class basic_surface final
{
 public:
  explicit basic_surface(maybe_owner<SDL_Surface*> surface) noexcept(detail::is_handle<T>)
      : mSurface{surface}
  {
    if constexpr (detail::is_owner<T>) {
      if (!mSurface) {
        throw exception{"Cannot create owning surface from null pointer!"};
      }
    }
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit basic_surface(const char* file) : mSurface{IMG_Load(file)}
  {
    if (!mSurface) {
      throw img_error{};
    }
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit basic_surface(const std::string& file) : basic_surface{file.c_str()}
  {}
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  basic_surface(const iarea& size, const pixel_format format)
      : mSurface{SDL_CreateRGBSurfaceWithFormat(0,
                                                size.width,
                                                size.height,
                                                0,
                                                to_underlying(format))}
  {
    if (!mSurface) {
      throw sdl_error{};
    }
  }
  basic_surface(const basic_surface& other) noexcept(detail::is_handle<T>)
  {
    if constexpr (detail::is_owner<T>) {
      copy(other);
    }
    else {
      mSurface = other.get();
    }
  }
  basic_surface(basic_surface&& other) noexcept = default;
  auto operator=(const basic_surface& other) noexcept(detail::is_handle<T>) -> basic_surface&
  {
    if (this != &other) {
      if constexpr (detail::is_owner<T>) {
        copy(other);
      }
      else {
        mSurface = other.get();
      }
    }
    return *this;
  }
  auto operator=(basic_surface&& other) noexcept -> basic_surface& = default;
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] static auto with_format(const char* file,
                                        const blend_mode mode,
                                        const pixel_format format) -> surface
  {
    assert(file);
    surface source{file};
    source.set_blend_mode(mode);
    return source.convert_to(format);
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] static auto with_format(const std::string& file,
                                        const blend_mode mode,
                                        const pixel_format format) -> surface
  {
    return with_format(file.c_str(), mode, format);
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] static auto from_bmp(const char* file) -> surface
  {
    assert(file);
    return surface{SDL_LoadBMP(file)};
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] static auto from_bmp(const std::string& file) -> surface
  {
    return from_bmp(file.c_str());
  }
  auto save_as_bmp(const char* file) const noexcept -> result
  {
    assert(file);
    return SDL_SaveBMP(get(), file) != -1;
  }
  auto save_as_bmp(const std::string& file) const noexcept -> result
  {
    return save_as_bmp(file.c_str());
  }
  auto save_as_png(const char* file) const noexcept -> result
  {
    assert(file);
    return IMG_SavePNG(get(), file) != -1;
  }
  auto save_as_png(const std::string& file) const noexcept -> result
  {
    return save_as_png(file.c_str());
  }
  auto save_as_jpg(const char* file, const int quality) const noexcept -> result
  {
    assert(file);
    return IMG_SaveJPG(get(), file, quality) != -1;
  }
  auto save_as_jpg(const std::string& file, const int quality) const noexcept -> result
  {
    return save_as_jpg(file.c_str(), quality);
  }
  [[nodiscard]] auto convert_to(const pixel_format format) const -> surface
  {
    if (auto* converted = SDL_ConvertSurfaceFormat(mSurface, to_underlying(format), 0)) {
      surface result{converted};
      result.set_blend_mode(get_blend_mode());
      return result;
    }
    else {
      throw sdl_error{};
    }
  }
  auto lock() noexcept -> result
  {
    if (must_lock()) {
      return SDL_LockSurface(mSurface) == 0;
    }
    else {
      return success;
    }
  }
  void unlock() noexcept
  {
    if (must_lock()) {
      SDL_UnlockSurface(mSurface);
    }
  }
  [[nodiscard]] auto must_lock() const noexcept -> bool { return SDL_MUSTLOCK(mSurface); }
  [[nodiscard]] auto pixel_data() noexcept -> void* { return mSurface->pixels; }
  [[nodiscard]] auto pixel_data() const noexcept -> const void* { return mSurface->pixels; }
  void set_alpha_mod(const uint8 alpha) noexcept { SDL_SetSurfaceAlphaMod(mSurface, alpha); }
  void set_color_mod(const color& color) noexcept
  {
    SDL_SetSurfaceColorMod(mSurface, color.red(), color.green(), color.blue());
  }
  void set_blend_mode(const blend_mode mode) noexcept
  {
    SDL_SetSurfaceBlendMode(mSurface, static_cast<SDL_BlendMode>(mode));
  }
  auto set_rle(const bool enabled) noexcept -> result
  {
    return SDL_SetSurfaceRLE(mSurface, enabled ? 1 : 0) == 0;
  }
  [[nodiscard]] auto alpha() const noexcept -> uint8
  {
    uint8 alpha{0xFF};
    SDL_GetSurfaceAlphaMod(mSurface, &alpha);
    return alpha;
  }
  [[nodiscard]] auto color_mod() const noexcept -> color
  {
    uint8 red{};
    uint8 green{};
    uint8 blue{};
    if (SDL_GetSurfaceColorMod(mSurface, &red, &green, &blue) == 0) {
      return {red, green, blue};
    }
    else {
      return colors::white;
    }
  }
  [[nodiscard]] auto get_blend_mode() const noexcept -> blend_mode
  {
    SDL_BlendMode mode{};
    SDL_GetSurfaceBlendMode(mSurface, &mode);
    return static_cast<blend_mode>(mode);
  }
  [[nodiscard]] auto width() const noexcept -> int { return mSurface->w; }
  [[nodiscard]] auto height() const noexcept -> int { return mSurface->h; }
  [[nodiscard]] auto size() const noexcept -> iarea { return {width(), height()}; }
  [[nodiscard]] auto pitch() const noexcept -> int { return mSurface->pitch; }
  [[nodiscard]] auto format_info() const noexcept -> pixel_format_info_handle
  {
    return pixel_format_info_handle{mSurface->format};
  }
  [[nodiscard]] auto clip() const noexcept -> irect { return irect{mSurface->clip_rect}; }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit operator bool() const noexcept
  {
    return mSurface != nullptr;
  }
  [[nodiscard]] auto get() const noexcept -> SDL_Surface* { return mSurface.get(); }
 private:
  detail::pointer<T, SDL_Surface> mSurface;
  void copy(const basic_surface& other) { mSurface.reset(other.duplicate_surface()); }
  [[nodiscard]] auto duplicate_surface() const -> owner<SDL_Surface*>
  {
    if (auto* copy = SDL_DuplicateSurface(mSurface)) {
      return copy;
    }
    else {
      throw sdl_error{};
    }
  }
};
template <typename T>
[[nodiscard]] auto to_string(const basic_surface<T>& surface) -> std::string
{
  return "surface(data: " + detail::address_of(surface.get()) +
         ", width: " + std::to_string(surface.width()) +
         ", height: " + std::to_string(surface.height()) + ")";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_surface<T>& surface) -> std::ostream&
{
  return stream << to_string(surface);
}
}
namespace cen {
}
namespace cen {
enum class music_type
{
  none = MUS_NONE,
  mp3 = MUS_MP3,
  wav = MUS_WAV,
  ogg = MUS_OGG,
  mod = MUS_MOD,
  midi = MUS_MID,
  cmd = MUS_CMD,
  flac = MUS_FLAC,
  opus = MUS_OPUS
};
[[nodiscard]] constexpr auto to_string(const music_type type) -> std::string_view
{
  switch (type) {
    case music_type::none:
      return "none";
    case music_type::mp3:
      return "mp3";
    case music_type::wav:
      return "wav";
    case music_type::ogg:
      return "ogg";
    case music_type::mod:
      return "mod";
    case music_type::midi:
      return "midi";
    case music_type::cmd:
      return "cmd";
    case music_type::flac:
      return "flac";
    case music_type::opus:
      return "opus";
    default:
      throw exception{"Did not recognize music type!"};
  }
}
inline auto operator<<(std::ostream& stream, const music_type type) -> std::ostream&
{
  return stream << to_string(type);
}
enum class fade_status
{
  none = MIX_NO_FADING,
  in = MIX_FADING_IN,
  out = MIX_FADING_OUT
};
[[nodiscard]] constexpr auto to_string(const fade_status status) -> std::string_view
{
  switch (status) {
    case fade_status::none:
      return "none";
    case fade_status::in:
      return "in";
    case fade_status::out:
      return "out";
    default:
      throw exception{"Did not recognize fade status!"};
  }
}
inline auto operator<<(std::ostream& stream, const fade_status status) -> std::ostream&
{
  return stream << to_string(status);
}
class music final
{
 public:
  using channel_index = int;
  using ms_type = millis<int>;
  using music_hook_callback = void (*)(void*, uint8*, int);
  inline constexpr static int forever = -1;
  explicit music(const char* file) : mMusic{Mix_LoadMUS(file)}
  {
    if (!mMusic) {
      throw mix_error{};
    }
  }
  explicit music(const std::string& file) : music{file.c_str()} {}
  auto play(const int iterations = 0) noexcept -> maybe<channel_index>
  {
    const auto channel = Mix_PlayMusic(mMusic.get(), detail::max(iterations, forever));
    if (channel != -1) {
      return channel;
    }
    else {
      return nothing;
    }
  }
  static void resume() noexcept { Mix_ResumeMusic(); }
  static void pause() noexcept { Mix_PauseMusic(); }
  static void halt() noexcept
  {
    Mix_HaltMusic();
  }
  [[nodiscard]] static auto is_playing() noexcept -> bool { return Mix_PlayingMusic(); }
  [[nodiscard]] static auto is_paused() noexcept -> bool { return Mix_PausedMusic(); }
  auto fade_in(const ms_type duration,
               const int iterations = 0) noexcept(noexcept(duration.count())) -> result
  {
    assert(duration.count() > 0);
    return Mix_FadeInMusic(mMusic.get(), detail::max(iterations, forever), duration.count()) ==
           0;
  }
  static auto fade_out(const ms_type duration) noexcept(noexcept(duration.count())) -> result
  {
    assert(duration.count() > 0);
    if (!is_fading()) {
      return Mix_FadeOutMusic(duration.count()) != 0;
    }
    else {
      return failure;
    }
  }
  [[nodiscard]] static auto get_fade_status() noexcept -> fade_status
  {
    return static_cast<fade_status>(Mix_FadingMusic());
  }
  [[nodiscard]] static auto is_fading() noexcept -> bool
  {
    const auto status = get_fade_status();
    return status == fade_status::in || status == fade_status::out;
  }
  [[nodiscard]] static auto is_fading_in() noexcept -> bool
  {
    return get_fade_status() == fade_status::in;
  }
  [[nodiscard]] static auto is_fading_out() noexcept -> bool
  {
    return get_fade_status() == fade_status::out;
  }
  static void rewind() noexcept { Mix_RewindMusic(); }
  static auto set_position(const double position) noexcept -> result
  {
    return Mix_SetMusicPosition(position) == 0;
  }
  static void set_volume(const int volume) noexcept
  {
    Mix_VolumeMusic(detail::clamp(volume, 0, MIX_MAX_VOLUME));
  }
  [[nodiscard]] static auto volume() noexcept -> int { return Mix_VolumeMusic(-1); }
  [[nodiscard]] constexpr static auto max_volume() noexcept -> int { return MIX_MAX_VOLUME; }
  template <typename T = void>
  static void set_hook(music_hook_callback callback, T* data = nullptr) noexcept
  {
    Mix_HookMusic(callback, data);
  }
  static void reset_hook() noexcept { set_hook(nullptr); }
  [[nodiscard]] static auto hook_data() noexcept -> void* { return Mix_GetMusicHookData(); }
  [[nodiscard]] static auto has_decoder(const char* name) noexcept -> bool
  {
    return Mix_HasMusicDecoder(name) == SDL_TRUE;
  }
  [[nodiscard]] static auto get_decoder(const int index) noexcept -> const char*
  {
    return Mix_GetMusicDecoder(index);
  }
  [[nodiscard]] static auto decoder_count() noexcept -> int
  {
    return Mix_GetNumMusicDecoders();
  }
  [[nodiscard]] auto type() const noexcept -> music_type
  {
    return static_cast<music_type>(Mix_GetMusicType(mMusic.get()));
  }
  [[nodiscard]] auto get() const noexcept -> Mix_Music* { return mMusic.get(); }
 private:
  managed_ptr<Mix_Music> mMusic;
};
[[nodiscard]] inline auto to_string(const music& music) -> std::string
{
  return "music(data: " + detail::address_of(music.get()) +
         ", volume: " + std::to_string(music::volume()) + ")";
}
inline auto operator<<(std::ostream& stream, const music& music) -> std::ostream&
{
  return stream << to_string(music);
}
template <typename T>
class basic_sound_effect;
using sound_effect = basic_sound_effect<detail::owner_tag>;
using sound_effect_handle = basic_sound_effect<detail::handle_tag>;
template <typename T>
class basic_sound_effect final
{
 public:
  using channel_index = int;
  using ms_type = millis<int>;
  inline constexpr static channel_index undefined_channel = -1;
  inline constexpr static int forever = -1;
  explicit basic_sound_effect(maybe_owner<Mix_Chunk*> sound) noexcept(detail::is_handle<T>)
      : mChunk{sound}
  {
    if constexpr (detail::is_owner<T>) {
      if (!mChunk) {
        throw mix_error{};
      }
    }
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit basic_sound_effect(const char* file) : mChunk{Mix_LoadWAV(file)}
  {
    if (!mChunk) {
      throw mix_error{};
    }
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit basic_sound_effect(const std::string& file) : basic_sound_effect{file.c_str()}
  {}
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit basic_sound_effect(const sound_effect& owner) noexcept : mChunk{owner.get()}
  {}
  auto play(const int iterations = 0) noexcept -> result
  {
    mChannel = Mix_PlayChannel(mChannel, mChunk.get(), detail::max(iterations, forever));
    return mChannel != -1;
  }
  void stop() noexcept
  {
    if (is_playing()) {
      Mix_Pause(mChannel);
      mChannel = undefined_channel;
    }
  }
  [[nodiscard]] auto is_playing() const noexcept -> bool
  {
    return (mChannel != undefined_channel) && Mix_Playing(mChannel);
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] static auto is_any_playing() noexcept -> bool
  {
    return Mix_Playing(undefined_channel);
  }
  void fade_in(const ms_type duration) noexcept(noexcept(duration.count()))
  {
    assert(duration.count() > 0);
    if (!is_playing()) {
      mChannel = Mix_FadeInChannel(mChannel, get(), 0, duration.count());
    }
  }
  void fade_out(const ms_type duration) noexcept(noexcept(duration.count()))
  {
    assert(duration.count() > 0);
    if (is_playing()) {
      Mix_FadeOutChannel(mChannel, duration.count());
    }
  }
  [[nodiscard]] auto is_fading() const noexcept -> bool
  {
    return is_playing() && Mix_FadingChannel(mChannel);
  }
  [[nodiscard]] auto channel() const noexcept -> maybe<channel_index>
  {
    if (mChannel != undefined_channel) {
      return mChannel;
    }
    else {
      return nothing;
    }
  }
  void set_volume(const int volume) noexcept
  {
    Mix_VolumeChunk(mChunk.get(), detail::clamp(volume, 0, max_volume()));
  }
  [[nodiscard]] auto volume() const noexcept -> int { return mChunk->volume; }
  [[nodiscard]] constexpr static auto max_volume() noexcept -> int { return MIX_MAX_VOLUME; }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] static auto has_decoder(const char* name) noexcept -> bool
  {
    return Mix_HasChunkDecoder(name) == SDL_TRUE;
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] static auto get_decoder(const int index) noexcept -> const char*
  {
    return Mix_GetChunkDecoder(index);
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] static auto decoder_count() noexcept -> int
  {
    return Mix_GetNumChunkDecoders();
  }
  [[nodiscard]] auto get() const noexcept -> Mix_Chunk* { return mChunk.get(); }
 private:
  detail::pointer<T, Mix_Chunk> mChunk;
  channel_index mChannel{undefined_channel};
};
template <typename T>
[[nodiscard]] auto to_string(const basic_sound_effect<T>& sound) -> std::string
{
  return "sound_effect(data: " + detail::address_of(sound.get()) +
         ", volume: " + std::to_string(sound.volume()) + ")";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_sound_effect<T>& sound) -> std::ostream&
{
  return stream << to_string(sound);
}
[[nodiscard]] inline auto get_sound(const int channel) noexcept -> sound_effect_handle
{
  return sound_effect_handle{Mix_GetChunk(channel)};
}
}
namespace cen {
enum class event_type : uint32
{
  first_event = SDL_FIRSTEVENT,
  last_event = SDL_LASTEVENT,
  quit = SDL_QUIT,
  app_terminating = SDL_APP_TERMINATING,
  app_low_memory = SDL_APP_LOWMEMORY,
  app_will_enter_background = SDL_APP_WILLENTERBACKGROUND,
  app_did_enter_background = SDL_APP_DIDENTERBACKGROUND,
  app_will_enter_foreground = SDL_APP_WILLENTERFOREGROUND,
  app_did_enter_foreground = SDL_APP_DIDENTERFOREGROUND,
  window = SDL_WINDOWEVENT,
  system = SDL_SYSWMEVENT,
  key_down = SDL_KEYDOWN,
  key_up = SDL_KEYUP,
  text_editing = SDL_TEXTEDITING,
  text_input = SDL_TEXTINPUT,
  keymap_changed = SDL_KEYMAPCHANGED,
  mouse_motion = SDL_MOUSEMOTION,
  mouse_button_down = SDL_MOUSEBUTTONDOWN,
  mouse_button_up = SDL_MOUSEBUTTONUP,
  mouse_wheel = SDL_MOUSEWHEEL,
  joy_axis_motion = SDL_JOYAXISMOTION,
  joy_ball_motion = SDL_JOYBALLMOTION,
  joy_hat_motion = SDL_JOYHATMOTION,
  joy_button_down = SDL_JOYBUTTONDOWN,
  joy_button_up = SDL_JOYBUTTONUP,
  joy_device_added = SDL_JOYDEVICEADDED,
  joy_device_removed = SDL_JOYDEVICEREMOVED,
  controller_axis_motion = SDL_CONTROLLERAXISMOTION,
  controller_button_down = SDL_CONTROLLERBUTTONDOWN,
  controller_button_up = SDL_CONTROLLERBUTTONUP,
  controller_device_added = SDL_CONTROLLERDEVICEADDED,
  controller_device_removed = SDL_CONTROLLERDEVICEREMOVED,
  controller_device_remapped = SDL_CONTROLLERDEVICEREMAPPED,
  finger_down = SDL_FINGERDOWN,
  finger_up = SDL_FINGERUP,
  finger_motion = SDL_FINGERMOTION,
  dollar_gesture = SDL_DOLLARGESTURE,
  dollar_record = SDL_DOLLARRECORD,
  multi_gesture = SDL_MULTIGESTURE,
  clipboard_update = SDL_CLIPBOARDUPDATE,
  drop_file = SDL_DROPFILE,
  drop_text = SDL_DROPTEXT,
  drop_begin = SDL_DROPBEGIN,
  drop_complete = SDL_DROPCOMPLETE,
  audio_device_added = SDL_AUDIODEVICEADDED,
  audio_device_removed = SDL_AUDIODEVICEREMOVED,
  sensor_update = SDL_SENSORUPDATE,
  render_targets_reset = SDL_RENDER_TARGETS_RESET,
  render_device_reset = SDL_RENDER_DEVICE_RESET,
  user = SDL_USEREVENT
};
[[nodiscard]] constexpr auto is_user_event(const event_type type) noexcept -> bool
{
  const auto raw = to_underlying(type);
  return raw >= SDL_USEREVENT && raw < SDL_LASTEVENT;
}
[[nodiscard]] constexpr auto to_string(const event_type type) -> std::string_view
{
  if (is_user_event(type)) {
    return "user";
  }
  switch (type) {
    case event_type::first_event:
      return "first_event";
    case event_type::last_event:
      return "last_event";
    case event_type::quit:
      return "quit";
    case event_type::app_terminating:
      return "app_terminating";
    case event_type::app_low_memory:
      return "app_low_memory";
    case event_type::app_will_enter_background:
      return "app_will_enter_background";
    case event_type::app_did_enter_background:
      return "app_did_enter_background";
    case event_type::app_will_enter_foreground:
      return "app_will_enter_foreground";
    case event_type::app_did_enter_foreground:
      return "app_did_enter_foreground";
    case event_type::window:
      return "window";
    case event_type::system:
      return "system";
    case event_type::key_down:
      return "key_down";
    case event_type::key_up:
      return "key_up";
    case event_type::text_editing:
      return "text_editing";
    case event_type::text_input:
      return "text_input";
    case event_type::keymap_changed:
      return "keymap_changed";
    case event_type::mouse_motion:
      return "mouse_motion";
    case event_type::mouse_button_down:
      return "mouse_button_down";
    case event_type::mouse_button_up:
      return "mouse_button_up";
    case event_type::mouse_wheel:
      return "mouse_wheel";
    case event_type::joy_axis_motion:
      return "joy_axis_motion";
    case event_type::joy_ball_motion:
      return "joy_ball_motion";
    case event_type::joy_hat_motion:
      return "joy_hat_motion";
    case event_type::joy_button_down:
      return "joy_button_down";
    case event_type::joy_button_up:
      return "joy_button_up";
    case event_type::joy_device_added:
      return "joy_device_added";
    case event_type::joy_device_removed:
      return "joy_device_removed";
    case event_type::controller_axis_motion:
      return "controller_axis_motion";
    case event_type::controller_button_down:
      return "controller_button_down";
    case event_type::controller_button_up:
      return "controller_button_up";
    case event_type::controller_device_added:
      return "controller_device_added";
    case event_type::controller_device_removed:
      return "controller_device_removed";
    case event_type::controller_device_remapped:
      return "controller_device_remapped";
    case event_type::finger_down:
      return "finger_down";
    case event_type::finger_up:
      return "finger_up";
    case event_type::finger_motion:
      return "finger_motion";
    case event_type::dollar_gesture:
      return "dollar_gesture";
    case event_type::dollar_record:
      return "dollar_record";
    case event_type::multi_gesture:
      return "multi_gesture";
    case event_type::clipboard_update:
      return "clipboard_update";
    case event_type::drop_file:
      return "drop_file";
    case event_type::drop_text:
      return "drop_text";
    case event_type::drop_begin:
      return "drop_begin";
    case event_type::drop_complete:
      return "drop_complete";
    case event_type::audio_device_added:
      return "audio_device_added";
    case event_type::audio_device_removed:
      return "audio_device_removed";
    case event_type::sensor_update:
      return "sensor_update";
    case event_type::render_targets_reset:
      return "render_targets_reset";
    case event_type::render_device_reset:
      return "render_device_reset";
    case event_type::user:
      return "user";
    default:
      throw exception{"Did not recognize event type!"};
  }
}
inline auto operator<<(std::ostream& stream, const event_type type) -> std::ostream&
{
  return stream << to_string(type);
}
template <typename T>
class event_base
{
 public:
  explicit event_base(const event_type type)
  {
    set_timestamp(u32ms{SDL_GetTicks()});
    set_type(type);
  }
  explicit event_base(const T& event) noexcept : mEvent{event} {}
  explicit event_base(T&& event) noexcept : mEvent{std::move(event)} {}
  void set_timestamp(const u32ms timestamp) noexcept(on_msvc)
  {
    mEvent.timestamp = timestamp.count();
  }
  void set_type(const event_type type) noexcept { mEvent.type = to_underlying(type); }
  [[nodiscard]] auto timestamp() const -> u32ms { return u32ms{mEvent.timestamp}; }
  [[nodiscard]] auto type() const noexcept -> event_type { return event_type{mEvent.type}; }
  [[nodiscard]] auto get() const noexcept -> const T& { return mEvent; }
 protected:
  T mEvent{};
};
template <typename T>
[[nodiscard]] auto as_sdl_event(const event_base<T>& event) -> SDL_Event;
}
namespace cen {
class audio_device_event final : public event_base<SDL_AudioDeviceEvent>
{
 public:
  audio_device_event() : event_base{event_type::audio_device_added} {}
  explicit audio_device_event(const SDL_AudioDeviceEvent& event) noexcept : event_base{event}
  {}
  void set_which(const uint32 which) noexcept { mEvent.which = which; }
  void set_capture(const bool capture) noexcept { mEvent.iscapture = capture; }
  [[nodiscard]] auto which() const noexcept -> uint32 { return mEvent.which; }
  [[nodiscard]] auto is_capture() const noexcept -> bool { return mEvent.iscapture; }
  [[nodiscard]] auto is_output() const noexcept -> bool { return !is_capture(); }
};
template <>
inline auto as_sdl_event(const event_base<SDL_AudioDeviceEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.adevice = event.get();
  return e;
}
}
namespace cen {
using thread_id = SDL_threadID;
enum class thread_priority
{
  low = SDL_THREAD_PRIORITY_LOW,
  normal = SDL_THREAD_PRIORITY_NORMAL,
  high = SDL_THREAD_PRIORITY_HIGH,
  critical = SDL_THREAD_PRIORITY_TIME_CRITICAL
};
[[nodiscard]] constexpr auto to_string(const thread_priority priority) -> std::string_view
{
  switch (priority) {
    case thread_priority::low:
      return "low";
    case thread_priority::normal:
      return "normal";
    case thread_priority::high:
      return "high";
    case thread_priority::critical:
      return "critical";
    default:
      throw exception{"Did not recognize thread priority!"};
  }
}
inline auto operator<<(std::ostream& stream, const thread_priority priority) -> std::ostream&
{
  return stream << to_string(priority);
}
enum class lock_status
{
  success = 0,
  timed_out = SDL_MUTEX_TIMEDOUT,
  error = -1
};
[[nodiscard]] constexpr auto to_string(const lock_status status) -> std::string_view
{
  switch (status) {
    case lock_status::success:
      return "success";
    case lock_status::timed_out:
      return "timed_out";
    case lock_status::error:
      return "error";
    default:
      throw exception{"Did not recognize lock status!"};
  }
}
inline auto operator<<(std::ostream& stream, const lock_status status) -> std::ostream&
{
  return stream << to_string(status);
}
class mutex final
{
 public:
  mutex() : mMutex{SDL_CreateMutex()}
  {
    if (!mMutex) {
      throw sdl_error{};
    }
  }
  auto lock() noexcept -> result { return SDL_LockMutex(data()) == 0; }
  auto try_lock() noexcept -> lock_status
  {
    return static_cast<lock_status>(SDL_TryLockMutex(data()));
  }
  auto unlock() noexcept -> result { return SDL_UnlockMutex(data()) == 0; }
  [[nodiscard]] auto data() noexcept -> SDL_mutex* { return mMutex.get(); }
  [[nodiscard]] auto data() const noexcept -> const SDL_mutex* { return mMutex.get(); }
 private:
  managed_ptr<SDL_mutex> mMutex;
};
class scoped_lock final
{
 public:
                           explicit scoped_lock(mutex& mutex) : mMutex{&mutex}
  {
    if (!mutex.lock()) {
      throw sdl_error{};
    }
  }
  scoped_lock(const scoped_lock&) = delete; scoped_lock& operator=(const scoped_lock&) = delete;
  scoped_lock(scoped_lock&&) = delete; scoped_lock& operator=(scoped_lock&&) = delete;
  ~scoped_lock() noexcept { mMutex->unlock(); }
 private:
  mutex* mMutex{};
};
class try_lock final
{
 public:
                           explicit try_lock(mutex& mutex) noexcept
      : mMutex{&mutex}
      , mStatus{mutex.try_lock()}
  {}
  try_lock(const try_lock&) = delete; try_lock& operator=(const try_lock&) = delete;
  ~try_lock() noexcept
  {
    if (mStatus == lock_status::success) {
      mMutex->unlock();
    }
  }
  [[nodiscard]] auto status() const noexcept -> lock_status { return mStatus; }
  [[nodiscard]] auto locked() const noexcept -> bool
  {
    return status() == lock_status::success;
  }
  [[nodiscard]] auto timed_out() const noexcept -> bool
  {
    return status() == lock_status::timed_out;
  }
  [[nodiscard]] auto failed() const noexcept -> bool { return status() == lock_status::error; }
  [[nodiscard]] explicit operator bool() const noexcept { return locked(); }
 private:
  mutex* mMutex{};
  lock_status mStatus{};
};
class condition final
{
 public:
  condition() : mCond{SDL_CreateCond()}
  {
    if (!mCond) {
      throw sdl_error{};
    }
  }
  auto signal() noexcept -> result { return SDL_CondSignal(mCond.get()) == 0; }
  auto broadcast() noexcept -> result { return SDL_CondBroadcast(mCond.get()) == 0; }
  auto wait(mutex& mutex) noexcept -> result
  {
    return SDL_CondWait(mCond.get(), mutex.data()) == 0;
  }
  auto wait(mutex& mutex, const u32ms duration) noexcept(noexcept(duration.count()))
      -> lock_status
  {
    return static_cast<lock_status>(
        SDL_CondWaitTimeout(mCond.get(), mutex.data(), duration.count()));
  }
 private:
  managed_ptr<SDL_cond> mCond;
};
class semaphore final
{
 public:
  explicit semaphore(const uint32 tokens) : mSemaphore{SDL_CreateSemaphore(tokens)}
  {
    if (!mSemaphore) {
      throw sdl_error{};
    }
  }
  auto acquire() noexcept -> result { return SDL_SemWait(mSemaphore.get()) == 0; }
  auto acquire(const u32ms duration) noexcept(noexcept(duration.count())) -> lock_status
  {
    return static_cast<lock_status>(SDL_SemWaitTimeout(mSemaphore.get(), duration.count()));
  }
  auto try_acquire() noexcept -> lock_status
  {
    return static_cast<lock_status>(SDL_SemTryWait(mSemaphore.get()));
  }
  auto release() noexcept -> result { return SDL_SemPost(mSemaphore.get()) == 0; }
  [[nodiscard]] auto count() const noexcept -> uint32
  {
    return SDL_SemValue(mSemaphore.get());
  }
 private:
  managed_ptr<SDL_sem> mSemaphore;
};
class thread final
{
 public:
                           explicit thread(SDL_ThreadFunction task,
                                           const char* name = "thread",
                                           void* data = nullptr)
      : mThread{SDL_CreateThread(task, name, data)}
  {
    if (!mThread) {
      throw sdl_error{};
    }
  }
  thread(const thread&) = delete; thread& operator=(const thread&) = delete;
  ~thread() noexcept
  {
    if (joinable()) {
      join();
    }
  }
  static void sleep(const u32ms duration) noexcept(noexcept(duration.count()))
  {
    SDL_Delay(duration.count());
  }
  static auto set_priority(const thread_priority priority) noexcept -> result
  {
    return SDL_SetThreadPriority(static_cast<SDL_ThreadPriority>(priority)) == 0;
  }
  auto join() noexcept -> int
  {
    if (mJoined || mDetached) {
      return 0;
    }
    int status{};
    SDL_WaitThread(mThread, &status);
    mJoined = true;
    assert(mDetached != mJoined);
    return status;
  }
  void detach() noexcept
  {
    if (mJoined || mDetached) {
      return;
    }
    SDL_DetachThread(mThread);
    mDetached = true;
    assert(mDetached != mJoined);
  }
  [[nodiscard]] static auto current_id() noexcept -> thread_id { return SDL_ThreadID(); }
  [[nodiscard]] auto id() const noexcept -> thread_id { return SDL_GetThreadID(mThread); }
  [[nodiscard]] auto name() const -> std::string { return SDL_GetThreadName(mThread); }
  [[nodiscard]] auto joinable() const noexcept -> bool { return !mJoined && !mDetached; }
  [[nodiscard]] auto joined() const noexcept -> bool { return mJoined; }
  [[nodiscard]] auto detached() const noexcept -> bool { return mDetached; }
  [[nodiscard]] auto data() noexcept -> SDL_Thread* { return mThread; }
  [[nodiscard]] auto data() const noexcept -> const SDL_Thread* { return mThread; }
 private:
  SDL_Thread* mThread{};
  bool mJoined{false};
  bool mDetached{false};
};
[[nodiscard]] inline auto to_string(const thread& thread) -> std::string
{
  return "thread(data: " + detail::address_of(thread.data()) + ", name: " + thread.name() +
         ", id: " + std::to_string(thread.id()) + ")";
}
inline auto operator<<(std::ostream& stream, const thread& thread) -> std::ostream&
{
  return stream << to_string(thread);
}
}
namespace cen {
enum class button_state : uint8
{
  released = SDL_RELEASED,
  pressed = SDL_PRESSED
};
[[nodiscard]] constexpr auto to_string(const button_state state) -> std::string_view
{
  switch (state) {
    case button_state::released:
      return "released";
    case button_state::pressed:
      return "pressed";
    default:
      throw exception{"Did not recognize button state!"};
  }
}
inline auto operator<<(std::ostream& stream, const button_state state) -> std::ostream&
{
  return stream << to_string(state);
}
}
namespace cen {
struct ball_axis_delta final
{
  int dx{};
  int dy{};
};
enum class joystick_type
{
  unknown = SDL_JOYSTICK_TYPE_UNKNOWN,
  game_controller = SDL_JOYSTICK_TYPE_GAMECONTROLLER,
  wheel = SDL_JOYSTICK_TYPE_WHEEL,
  arcade_stick = SDL_JOYSTICK_TYPE_ARCADE_STICK,
  flight_stick = SDL_JOYSTICK_TYPE_FLIGHT_STICK,
  dance_pad = SDL_JOYSTICK_TYPE_DANCE_PAD,
  guitar = SDL_JOYSTICK_TYPE_GUITAR,
  drum_kit = SDL_JOYSTICK_TYPE_DRUM_KIT,
  arcade_pad = SDL_JOYSTICK_TYPE_ARCADE_PAD,
  throttle = SDL_JOYSTICK_TYPE_THROTTLE
};
[[nodiscard]] constexpr auto to_string(const joystick_type type) -> std::string_view
{
  switch (type) {
    case joystick_type::unknown:
      return "unknown";
    case joystick_type::game_controller:
      return "game_controller";
    case joystick_type::wheel:
      return "wheel";
    case joystick_type::arcade_stick:
      return "arcade_stick";
    case joystick_type::flight_stick:
      return "flight_stick";
    case joystick_type::dance_pad:
      return "dance_pad";
    case joystick_type::guitar:
      return "guitar";
    case joystick_type::drum_kit:
      return "drum_kit";
    case joystick_type::arcade_pad:
      return "arcade_pad";
    case joystick_type::throttle:
      return "throttle";
    default:
      throw exception{"Did not recognize joystick type!"};
  }
}
inline auto operator<<(std::ostream& stream, const joystick_type type) -> std::ostream&
{
  return stream << to_string(type);
}
enum class joystick_power
{
  unknown = SDL_JOYSTICK_POWER_UNKNOWN,
  empty = SDL_JOYSTICK_POWER_EMPTY,
  low = SDL_JOYSTICK_POWER_LOW,
  medium = SDL_JOYSTICK_POWER_MEDIUM,
  full = SDL_JOYSTICK_POWER_FULL,
  wired = SDL_JOYSTICK_POWER_WIRED,
  max = SDL_JOYSTICK_POWER_MAX
};
[[nodiscard]] constexpr auto to_string(const joystick_power power) -> std::string_view
{
  switch (power) {
    case joystick_power::unknown:
      return "unknown";
    case joystick_power::empty:
      return "empty";
    case joystick_power::low:
      return "low";
    case joystick_power::medium:
      return "medium";
    case joystick_power::full:
      return "full";
    case joystick_power::wired:
      return "wired";
    case joystick_power::max:
      return "max";
    default:
      throw exception{"Did not recognize joystick power!"};
  }
}
inline auto operator<<(std::ostream& stream, const joystick_power power) -> std::ostream&
{
  return stream << to_string(power);
}
enum class hat_state : uint8
{
  centered = SDL_HAT_CENTERED,
  up = SDL_HAT_UP,
  right = SDL_HAT_RIGHT,
  down = SDL_HAT_DOWN,
  left = SDL_HAT_LEFT,
  right_up = SDL_HAT_RIGHTUP,
  right_down = SDL_HAT_RIGHTDOWN,
  left_up = SDL_HAT_LEFTUP,
  left_down = SDL_HAT_LEFTDOWN,
};
[[nodiscard]] constexpr auto to_string(const hat_state state) -> std::string_view
{
  switch (state) {
    case hat_state::centered:
      return "centered";
    case hat_state::up:
      return "up";
    case hat_state::right:
      return "right";
    case hat_state::down:
      return "down";
    case hat_state::left:
      return "left";
    case hat_state::right_up:
      return "right_up";
    case hat_state::right_down:
      return "right_down";
    case hat_state::left_up:
      return "left_up";
    case hat_state::left_down:
      return "left_down";
    default:
      throw exception{"Did not recognize hat state!"};
  }
}
inline auto operator<<(std::ostream& stream, const hat_state state) -> std::ostream&
{
  return stream << to_string(state);
}
template <typename T>
class basic_joystick;
using joystick = basic_joystick<detail::owner_tag>;
using joystick_handle = basic_joystick<detail::handle_tag>;
template <typename T>
class basic_joystick final
{
 public:
  using device_index = int;
  using id_type = SDL_JoystickID;
  using guid_type = SDL_JoystickGUID;
  explicit basic_joystick(maybe_owner<SDL_Joystick*> joystick) noexcept(detail::is_handle<T>)
      : mJoystick{joystick}
  {
    if constexpr (detail::is_owner<T>) {
      if (!mJoystick) {
        throw exception{"Cannot create joystick from null pointer!"};
      }
    }
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit basic_joystick(const device_index index = 0) : mJoystick{SDL_JoystickOpen(index)}
  {
    if (!mJoystick) {
      throw sdl_error{};
    }
  }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit basic_joystick(const joystick& owner) noexcept : mJoystick{owner.get()}
  {}
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  [[nodiscard]] static auto from_id(const id_type id) noexcept -> joystick_handle
  {
    return joystick_handle{SDL_JoystickFromInstanceID(id)};
  }
  [[nodiscard]] auto name() const noexcept -> const char*
  {
    return SDL_JoystickName(mJoystick);
  }
  [[nodiscard]] static auto name(const device_index index) noexcept -> const char*
  {
    return SDL_JoystickNameForIndex(index);
  }
  [[nodiscard]] auto type() const noexcept -> joystick_type
  {
    return static_cast<joystick_type>(SDL_JoystickGetType(mJoystick));
  }
  [[nodiscard]] static auto type(const device_index index) noexcept -> joystick_type
  {
    return static_cast<joystick_type>(SDL_JoystickGetDeviceType(index));
  }
  [[nodiscard]] auto vendor() const noexcept -> maybe<uint16>
  {
    const auto vendor = SDL_JoystickGetVendor(mJoystick);
    if (vendor != 0) {
      return vendor;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] static auto vendor(const device_index index) noexcept -> maybe<uint16>
  {
    const auto vendor = SDL_JoystickGetDeviceVendor(index);
    if (vendor != 0) {
      return vendor;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto product() const noexcept -> maybe<uint16>
  {
    const auto product = SDL_JoystickGetProduct(mJoystick);
    if (product != 0) {
      return product;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] static auto product(const device_index index) noexcept -> maybe<uint16>
  {
    const auto product = SDL_JoystickGetDeviceProduct(index);
    if (product != 0) {
      return product;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto product_version() const noexcept -> maybe<uint16>
  {
    const auto version = SDL_JoystickGetProductVersion(mJoystick);
    if (version != 0) {
      return version;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] static auto product_version(const device_index index) noexcept -> maybe<uint16>
  {
    const auto version = SDL_JoystickGetDeviceProductVersion(index);
    if (version != 0) {
      return version;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto guid() noexcept -> guid_type { return SDL_JoystickGetGUID(mJoystick); }
  [[nodiscard]] static auto guid(const device_index index) noexcept -> guid_type
  {
    return SDL_JoystickGetDeviceGUID(index);
  }
  [[nodiscard]] auto button_count() const noexcept -> int
  {
    return SDL_JoystickNumButtons(mJoystick);
  }
  [[nodiscard]] auto hat_count() const noexcept -> int
  {
    return SDL_JoystickNumHats(mJoystick);
  }
  [[nodiscard]] auto axis_count() const noexcept -> int
  {
    return SDL_JoystickNumAxes(mJoystick);
  }
  [[nodiscard]] auto trackball_count() const noexcept -> int
  {
    return SDL_JoystickNumBalls(mJoystick);
  }
  [[nodiscard]] auto id() const noexcept -> id_type
  {
    return SDL_JoystickInstanceID(mJoystick);
  }
  [[nodiscard]] static auto id(const device_index index) noexcept -> maybe<id_type>
  {
    const auto id = SDL_JoystickGetDeviceInstanceID(index);
    if (id != -1) {
      return id;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto attached() const noexcept -> bool
  {
    return SDL_JoystickGetAttached(mJoystick);
  }
  [[nodiscard]] auto power() const noexcept -> joystick_power
  {
    return static_cast<joystick_power>(SDL_JoystickCurrentPowerLevel(mJoystick));
  }
  auto rumble(const uint16 lo,
              const uint16 hi,
              const u32ms duration) noexcept(noexcept(duration.count())) -> result
  {
    return SDL_JoystickRumble(mJoystick, lo, hi, duration.count()) == 0;
  }
  auto stop_rumble() -> result { return rumble(0, 0, u32ms::zero()); }
  [[nodiscard]] auto query_button(const int button) const noexcept -> button_state
  {
    return static_cast<button_state>(SDL_JoystickGetButton(mJoystick, button));
  }
  [[nodiscard]] auto query_hat(const int hat) const noexcept -> hat_state
  {
    return static_cast<hat_state>(SDL_JoystickGetHat(mJoystick, hat));
  }
  [[nodiscard]] auto query_axis(const int axis) const noexcept -> int16
  {
    return SDL_JoystickGetAxis(mJoystick, axis);
  }
  [[nodiscard]] auto axis_initial_state(const int axis) const noexcept -> maybe<int16>
  {
    int16 state{};
    if (SDL_JoystickGetAxisInitialState(mJoystick, axis, &state)) {
      return state;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto get_ball_axis_delta(const int ball) const noexcept
      -> maybe<ball_axis_delta>
  {
    ball_axis_delta change{};
    if (SDL_JoystickGetBall(mJoystick, ball, &change.dx, &change.dy) == 0) {
      return change;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto player_index() const noexcept -> maybe<int>
  {
    const auto index = SDL_JoystickGetPlayerIndex(mJoystick);
    if (index != -1) {
      return index;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] static auto player_index(const device_index index) noexcept -> maybe<int>
  {
    const auto player = SDL_JoystickGetDevicePlayerIndex(index);
    if (player != -1) {
      return player;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] static auto to_guid(const char* str) noexcept -> guid_type
  {
    assert(str);
    return SDL_JoystickGetGUIDFromString(str);
  }
  [[nodiscard]] static auto to_guid(const std::string& str) noexcept -> guid_type
  {
    return to_guid(str.c_str());
  }
  static void lock() noexcept { SDL_LockJoysticks(); }
  static void unlock() noexcept { SDL_UnlockJoysticks(); }
  static void update() noexcept { SDL_JoystickUpdate(); }
  static void set_polling(const bool enabled) noexcept
  {
    SDL_JoystickEventState(enabled ? SDL_ENABLE : SDL_DISABLE);
  }
  [[nodiscard]] static auto polling() noexcept -> bool
  {
    return SDL_JoystickEventState(SDL_QUERY);
  }
  [[nodiscard]] static auto count() noexcept -> maybe<int>
  {
    const auto result = SDL_NumJoysticks();
    if (result >= 0) {
      return result;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] constexpr static auto axis_max() noexcept -> int16
  {
    return SDL_JOYSTICK_AXIS_MAX;
  }
  [[nodiscard]] constexpr static auto axis_min() noexcept -> int16
  {
    return SDL_JOYSTICK_AXIS_MIN;
  }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit operator bool() const noexcept
  {
    return mJoystick != nullptr;
  }
  [[nodiscard]] auto get() const noexcept -> SDL_Joystick* { return mJoystick.get(); }
 private:
  detail::pointer<T, SDL_Joystick> mJoystick;
};
template <typename T>
[[nodiscard]] auto to_string(const basic_joystick<T>& joystick) -> std::string
{
  const char* serial{};
  if constexpr (detail::sdl_version_at_least(2, 0, 14)) {
    serial = joystick.serial();
  }
  return "joystick(data: " + detail::address_of(joystick.get()) +
         ", id: " + std::to_string(joystick.id()) + ", name: '" + str_or_na(joystick.name()) +
         ", serial: " + str_or_na(serial) + "')";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_joystick<T>& joystick) -> std::ostream&
{
  return stream << to_string(joystick);
}
}
namespace cen {
using sensor_id = SDL_SensorID;
[[nodiscard]] constexpr auto standard_gravity() noexcept -> float
{
  return SDL_STANDARD_GRAVITY;
}
enum class sensor_type
{
  invalid = SDL_SENSOR_INVALID,
  unknown = SDL_SENSOR_UNKNOWN,
  accelerometer = SDL_SENSOR_ACCEL,
  gyroscope = SDL_SENSOR_GYRO
};
[[nodiscard]] constexpr auto to_string(const sensor_type type) -> std::string_view
{
  switch (type) {
    case sensor_type::invalid:
      return "invalid";
    case sensor_type::unknown:
      return "unknown";
    case sensor_type::accelerometer:
      return "accelerometer";
    case sensor_type::gyroscope:
      return "gyroscope";
    default:
      throw exception{"Did not recognize sensor type!"};
  }
}
inline auto operator<<(std::ostream& stream, const sensor_type type) -> std::ostream&
{
  return stream << to_string(type);
}
template <typename T>
class basic_sensor;
using sensor = basic_sensor<detail::owner_tag>;
using sensor_handle = basic_sensor<detail::handle_tag>;
template <typename T>
class basic_sensor final
{
 public:
  using device_index = int;
  using size_type = usize;
  template <size_type Size>
  using data_type = std::array<float, Size>;
  explicit basic_sensor(maybe_owner<SDL_Sensor*> sensor) noexcept(detail::is_handle<T>)
      : mSensor{sensor}
  {
    if constexpr (detail::is_owner<T>) {
      if (!mSensor) {
        throw exception{"Null sensor pointer!"};
      }
    }
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit basic_sensor(const device_index index = 0) : mSensor{SDL_SensorOpen(index)}
  {
    if (!mSensor) {
      throw sdl_error{};
    }
  }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit basic_sensor(const sensor& owner) noexcept : mSensor{owner.get()}
  {}
  [[nodiscard]] auto name() const noexcept -> const char*
  {
    return SDL_SensorGetName(mSensor);
  }
  [[nodiscard]] static auto name(const device_index index) noexcept -> const char*
  {
    return SDL_SensorGetDeviceName(index);
  }
  [[nodiscard]] auto type() const noexcept -> sensor_type
  {
    return static_cast<sensor_type>(SDL_SensorGetType(mSensor));
  }
  [[nodiscard]] static auto type(const device_index index) noexcept -> sensor_type
  {
    return static_cast<sensor_type>(SDL_SensorGetDeviceType(index));
  }
  [[nodiscard]] auto non_portable_type() const noexcept -> maybe<int>
  {
    const auto type = SDL_SensorGetNonPortableType(mSensor);
    if (type != -1) {
      return type;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] static auto non_portable_type(const device_index index) noexcept -> maybe<int>
  {
    const auto type = SDL_SensorGetDeviceNonPortableType(index);
    if (type != -1) {
      return type;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto id() const noexcept -> maybe<sensor_id>
  {
    const auto id = SDL_SensorGetInstanceID(mSensor);
    if (id != -1) {
      return id;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] static auto id(const device_index index) noexcept -> maybe<sensor_id>
  {
    const auto id = SDL_SensorGetDeviceInstanceID(index);
    if (id != -1) {
      return id;
    }
    else {
      return nothing;
    }
  }
  template <size_type Size>
  [[nodiscard]] auto data() const noexcept -> maybe<data_type<Size>>
  {
    data_type<Size> array{};
    if (SDL_SensorGetData(mSensor, array.data(), isize(array)) != -1) {
      return array;
    }
    else {
      return nothing;
    }
  }
  static void update() noexcept { SDL_SensorUpdate(); }
  [[nodiscard]] static auto count() noexcept -> int { return SDL_NumSensors(); }
  [[nodiscard]] auto get() const noexcept -> SDL_Sensor* { return mSensor.get(); }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit operator bool() const noexcept
  {
    return mSensor != nullptr;
  }
 private:
  detail::pointer<T, SDL_Sensor> mSensor;
};
template <typename T>
[[nodiscard]] auto to_string(const basic_sensor<T>& sensor) -> std::string
{
  return "sensor(data: " + detail::address_of(sensor.get()) +
         ", id: " + std::to_string(sensor.id().value()) + ", name: '" +
         str_or_na(sensor.name()) + "')";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_sensor<T>& sensor) -> std::ostream&
{
  return stream << to_string(sensor);
}
}
namespace cen {
enum class controller_button
{
  invalid = SDL_CONTROLLER_BUTTON_INVALID,
  a = SDL_CONTROLLER_BUTTON_A,
  b = SDL_CONTROLLER_BUTTON_B,
  x = SDL_CONTROLLER_BUTTON_X,
  y = SDL_CONTROLLER_BUTTON_Y,
  back = SDL_CONTROLLER_BUTTON_BACK,
  guide = SDL_CONTROLLER_BUTTON_GUIDE,
  start = SDL_CONTROLLER_BUTTON_START,
  left_stick = SDL_CONTROLLER_BUTTON_LEFTSTICK,
  right_stick = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
  left_shoulder = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
  right_shoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
  dpad_up = SDL_CONTROLLER_BUTTON_DPAD_UP,
  dpad_down = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
  dpad_left = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
  dpad_right = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
  max = SDL_CONTROLLER_BUTTON_MAX
};
[[nodiscard]] constexpr auto to_string(const controller_button button) -> std::string_view
{
  switch (button) {
    case controller_button::invalid:
      return "invalid";
    case controller_button::a:
      return "a";
    case controller_button::b:
      return "b";
    case controller_button::x:
      return "x";
    case controller_button::y:
      return "y";
    case controller_button::back:
      return "back";
    case controller_button::guide:
      return "guide";
    case controller_button::start:
      return "start";
    case controller_button::left_stick:
      return "left_stick";
    case controller_button::right_stick:
      return "right_stick";
    case controller_button::left_shoulder:
      return "left_shoulder";
    case controller_button::right_shoulder:
      return "right_shoulder";
    case controller_button::dpad_up:
      return "dpad_up";
    case controller_button::dpad_down:
      return "dpad_down";
    case controller_button::dpad_left:
      return "dpad_left";
    case controller_button::dpad_right:
      return "dpad_right";
    case controller_button::max:
      return "max";
    default:
      throw exception{"Did not recognize controller button!"};
  }
}
inline auto operator<<(std::ostream& stream, const controller_button button) -> std::ostream&
{
  return stream << to_string(button);
}
enum class controller_axis
{
  invalid = SDL_CONTROLLER_AXIS_INVALID,
  left_x = SDL_CONTROLLER_AXIS_LEFTX,
  left_y = SDL_CONTROLLER_AXIS_LEFTY,
  right_x = SDL_CONTROLLER_AXIS_RIGHTX,
  right_y = SDL_CONTROLLER_AXIS_RIGHTY,
  trigger_left = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
  trigger_right = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
  max = SDL_CONTROLLER_AXIS_MAX
};
[[nodiscard]] constexpr auto to_string(const controller_axis axis) -> std::string_view
{
  switch (axis) {
    case controller_axis::invalid:
      return "invalid";
    case controller_axis::left_x:
      return "left_x";
    case controller_axis::left_y:
      return "left_y";
    case controller_axis::right_x:
      return "right_x";
    case controller_axis::right_y:
      return "right_y";
    case controller_axis::trigger_left:
      return "trigger_left";
    case controller_axis::trigger_right:
      return "trigger_right";
    case controller_axis::max:
      return "max";
    default:
      throw exception{"Did not recognize controller axis!"};
  }
}
inline auto operator<<(std::ostream& stream, const controller_axis axis) -> std::ostream&
{
  return stream << to_string(axis);
}
enum class controller_bind_type
{
  none = SDL_CONTROLLER_BINDTYPE_NONE,
  button = SDL_CONTROLLER_BINDTYPE_BUTTON,
  axis = SDL_CONTROLLER_BINDTYPE_AXIS,
  hat = SDL_CONTROLLER_BINDTYPE_HAT
};
[[nodiscard]] constexpr auto to_string(const controller_bind_type type) -> std::string_view
{
  switch (type) {
    case controller_bind_type::none:
      return "none";
    case controller_bind_type::button:
      return "button";
    case controller_bind_type::axis:
      return "axis";
    case controller_bind_type::hat:
      return "hat";
    default:
      throw exception{"Did not recognize controller bind type!"};
  }
}
inline auto operator<<(std::ostream& stream, const controller_bind_type type) -> std::ostream&
{
  return stream << to_string(type);
}
enum class controller_mapping_result
{
  error,
  updated,
  added
};
[[nodiscard]] inline auto to_string(const controller_mapping_result result) -> std::string_view
{
  switch (result) {
    case controller_mapping_result::error:
      return "error";
    case controller_mapping_result::updated:
      return "updated";
    case controller_mapping_result::added:
      return "added";
    default:
      throw exception{"Did not recognize controller mapping result!"};
  }
}
inline auto operator<<(std::ostream& stream, const controller_mapping_result result)
    -> std::ostream&
{
  return stream << to_string(result);
}
struct controller_finger_state final
{
  button_state state{};
  float x{};
  float y{};
  float pressure{};
};
template <typename T>
class basic_controller;
using controller = basic_controller<detail::owner_tag>;
using controller_handle = basic_controller<detail::handle_tag>;
template <typename T>
class basic_controller final
{
 public:
  using mapping_index = int;
  using joystick_index = int;
  using player_index = int;
  explicit basic_controller(maybe_owner<SDL_GameController*> controller) noexcept(detail::is_handle<T>)
      : mController{controller}
  {
    if constexpr (detail::is_owner<T>) {
      if (!mController) {
        throw exception{"Cannot create controller from null pointer!"};
      }
    }
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit basic_controller(const joystick_index index = 0)
      : mController{SDL_GameControllerOpen(index)}
  {
    if (!mController) {
      throw sdl_error{};
    }
  }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit basic_controller(const controller& owner) noexcept : mController{owner.get()}
  {}
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] static auto from_joystick(const SDL_JoystickID id) -> basic_controller
  {
    if (auto* ptr = SDL_GameControllerFromInstanceID(id)) {
      return basic_controller{ptr};
    }
    else {
      throw sdl_error{};
    }
  }
  static void update() { SDL_GameControllerUpdate(); }
  static void set_polling(const bool polling) noexcept
  {
    SDL_GameControllerEventState(polling ? SDL_ENABLE : SDL_DISABLE);
  }
  [[nodiscard]] static auto polling() noexcept -> bool
  {
    return SDL_GameControllerEventState(SDL_QUERY);
  }
  [[nodiscard]] static auto supported(const joystick_index index) noexcept -> bool
  {
    return SDL_IsGameController(index) == SDL_TRUE;
  }
  [[nodiscard]] static auto count() noexcept -> int
  {
    const auto joysticks = SDL_NumJoysticks();
    auto amount = 0;
    for (auto i = 0; i < joysticks; ++i) {
      if (supported(i)) {
        ++amount;
      }
    }
    return amount;
  }
  auto rumble(const uint16 lo,
              const uint16 hi,
              const u32ms duration) noexcept(noexcept(duration.count())) -> result
  {
    return SDL_GameControllerRumble(mController, lo, hi, duration.count()) == 0;
  }
  auto stop_rumble() -> result { return rumble(0, 0, u32ms::zero()); }
  [[nodiscard]] auto state(const controller_button button) const noexcept -> button_state
  {
    const auto state =
        SDL_GameControllerGetButton(mController,
                                    static_cast<SDL_GameControllerButton>(button));
    return static_cast<button_state>(state);
  }
  [[nodiscard]] auto pressed(const controller_button button) const noexcept -> bool
  {
    return state(button) == button_state::pressed;
  }
  [[nodiscard]] auto released(const controller_button button) const noexcept -> bool
  {
    return state(button) == button_state::released;
  }
  [[nodiscard]] static auto button(const char* str) noexcept -> controller_button
  {
    assert(str);
    return static_cast<controller_button>(SDL_GameControllerGetButtonFromString(str));
  }
  [[nodiscard]] static auto button(const std::string& str) noexcept -> controller_button
  {
    return button(str.c_str());
  }
  [[nodiscard]] static auto stringify(const controller_button button) noexcept -> const char*
  {
    return SDL_GameControllerGetStringForButton(static_cast<SDL_GameControllerButton>(button));
  }
  [[nodiscard]] auto binding(const controller_button button) noexcept
      -> maybe<SDL_GameControllerButtonBind>
  {
    const auto result =
        SDL_GameControllerGetBindForButton(mController,
                                           static_cast<SDL_GameControllerButton>(button));
    if (result.bindType != SDL_CONTROLLER_BINDTYPE_NONE) {
      return result;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto axis(const controller_axis axis) const noexcept -> int16
  {
    return SDL_GameControllerGetAxis(mController, static_cast<SDL_GameControllerAxis>(axis));
  }
  [[nodiscard]] static auto axis(const char* str) noexcept -> controller_axis
  {
    assert(str);
    return static_cast<controller_axis>(SDL_GameControllerGetAxisFromString(str));
  }
  [[nodiscard]] static auto axis(const std::string& str) noexcept -> controller_axis
  {
    return axis(str.c_str());
  }
  [[nodiscard]] static auto stringify(const controller_axis axis) noexcept -> const char*
  {
    return SDL_GameControllerGetStringForAxis(static_cast<SDL_GameControllerAxis>(axis));
  }
  [[nodiscard]] auto binding(const controller_axis axis) const
      -> maybe<SDL_GameControllerButtonBind>
  {
    const auto result =
        SDL_GameControllerGetBindForAxis(mController,
                                         static_cast<SDL_GameControllerAxis>(axis));
    if (result.bindType != SDL_CONTROLLER_BINDTYPE_NONE) {
      return result;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto mapping() const noexcept -> sdl_string
  {
    return sdl_string{SDL_GameControllerMapping(mController)};
  }
  [[nodiscard]] static auto mapping(const joystick_index index) noexcept -> sdl_string
  {
    return sdl_string{SDL_GameControllerMappingForDeviceIndex(index)};
  }
  [[nodiscard]] static auto mapping(const SDL_JoystickGUID guid) noexcept -> sdl_string
  {
    return sdl_string{SDL_GameControllerMappingForGUID(guid)};
  }
  [[nodiscard]] static auto mapping_by_index(const mapping_index index) noexcept -> sdl_string
  {
    return sdl_string{SDL_GameControllerMappingForIndex(index)};
  }
  [[nodiscard]] auto name() const noexcept -> const char*
  {
    return SDL_GameControllerName(mController);
  }
  [[nodiscard]] auto vendor() const noexcept -> maybe<uint16>
  {
    const auto id = SDL_GameControllerGetVendor(mController);
    if (id != 0) {
      return id;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto product() const noexcept -> maybe<uint16>
  {
    const auto id = SDL_GameControllerGetProduct(mController);
    if (id != 0) {
      return id;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto product_version() const noexcept -> maybe<uint16>
  {
    const auto id = SDL_GameControllerGetProductVersion(mController);
    if (id != 0) {
      return id;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto connected() const noexcept -> bool
  {
    return SDL_GameControllerGetAttached(mController) == SDL_TRUE;
  }
  [[nodiscard]] auto index() const noexcept -> maybe<player_index>
  {
    const auto result = SDL_GameControllerGetPlayerIndex(mController);
    if (result != -1) {
      return result;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto get_joystick() noexcept -> joystick_handle
  {
    return joystick_handle{SDL_GameControllerGetJoystick(mController)};
  }
  [[nodiscard]] auto get() const noexcept -> SDL_GameController* { return mController.get(); }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit operator bool() const noexcept
  {
    return mController != nullptr;
  }
 private:
  detail::pointer<T, SDL_GameController> mController;
};
inline auto add_controller_mapping(const char* mapping) noexcept -> controller_mapping_result
{
  assert(mapping);
  const auto result = SDL_GameControllerAddMapping(mapping);
  if (result == 1) {
    return controller_mapping_result::added;
  }
  else if (result == 0) {
    return controller_mapping_result::updated;
  }
  else {
    return controller_mapping_result::error;
  }
}
inline auto add_controller_mapping(const std::string& mapping) noexcept
    -> controller_mapping_result
{
  return add_controller_mapping(mapping.c_str());
}
inline auto load_controller_mappings(const char* file) noexcept -> maybe<int>
{
  assert(file);
  const auto result = SDL_GameControllerAddMappingsFromFile(file);
  if (result != -1) {
    return result;
  }
  else {
    return nothing;
  }
}
inline auto load_controller_mappings(const std::string& file) noexcept -> maybe<int>
{
  return load_controller_mappings(file.c_str());
}
[[nodiscard]] inline auto controller_mapping_count() noexcept -> int
{
  return SDL_GameControllerNumMappings();
}
template <typename T>
[[nodiscard]] auto to_string(const basic_controller<T>& controller) -> std::string
{
  const auto* name = controller.name();
  const char* serial{};
  if constexpr (detail::sdl_version_at_least(2, 0, 14)) {
    serial = controller.serial();
  }
  return "controller(data: " + detail::address_of(controller.get()) + ", name: '" +
         str_or_na(name) + "', serial: '" + str_or_na(serial) + "')";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_controller<T>& controller) -> std::ostream&
{
  return stream << to_string(controller);
}
}
namespace cen::detail {
template <typename T, usize Size>
constexpr void assign(const std::array<T, Size>& array, bounded_array_ref<T, Size> out)
{
  usize index = 0;
  for (auto&& value : array) {
    out[index] = value;
    ++index;
  }
}
template <typename T, usize Size>
[[nodiscard]] constexpr auto to_array(bounded_array_ref<const T, Size> data)
    -> std::array<T, Size>
{
  std::array<T, Size> array;
  for (usize i = 0; i < Size; ++i) {
    array[i] = data[i];
  }
  return array;
}
}
namespace cen {
class controller_axis_event final : public event_base<SDL_ControllerAxisEvent>
{
 public:
  controller_axis_event() : event_base{event_type::controller_axis_motion} {}
  explicit controller_axis_event(const SDL_ControllerAxisEvent& event) noexcept
      : event_base{event}
  {}
  void set_which(const SDL_JoystickID which) noexcept { mEvent.which = which; }
  void set_axis(const controller_axis axis) noexcept
  {
    mEvent.axis = static_cast<uint8>(axis);
  }
  void set_value(const int16 value) noexcept { mEvent.value = value; }
  [[nodiscard]] auto which() const noexcept -> SDL_JoystickID { return mEvent.which; }
  [[nodiscard]] auto axis() const noexcept -> controller_axis
  {
    return static_cast<controller_axis>(mEvent.axis);
  }
  [[nodiscard]] auto value() const noexcept -> int16 { return mEvent.value; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_ControllerAxisEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.caxis = event.get();
  return e;
}
class controller_button_event final : public event_base<SDL_ControllerButtonEvent>
{
 public:
  controller_button_event() : event_base{event_type::controller_button_down} {}
  explicit controller_button_event(const SDL_ControllerButtonEvent& event) noexcept
      : event_base{event}
  {}
  void set_which(const SDL_JoystickID id) noexcept { mEvent.which = id; }
  void set_button(const controller_button button) noexcept
  {
    mEvent.button = static_cast<uint8>(button);
  }
  void set_state(const button_state state) noexcept { mEvent.state = to_underlying(state); }
  [[nodiscard]] auto which() const noexcept -> SDL_JoystickID { return mEvent.which; }
  [[nodiscard]] auto button() const noexcept -> controller_button
  {
    return static_cast<controller_button>(mEvent.button);
  }
  [[nodiscard]] auto state() const noexcept -> button_state
  {
    return static_cast<button_state>(mEvent.state);
  }
  [[nodiscard]] auto is_pressed() const noexcept -> bool
  {
    return state() == button_state::pressed;
  }
  [[nodiscard]] auto is_released() const noexcept -> bool
  {
    return state() == button_state::released;
  }
};
template <>
inline auto as_sdl_event(const event_base<SDL_ControllerButtonEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.cbutton = event.get();
  return e;
}
class controller_device_event final : public event_base<SDL_ControllerDeviceEvent>
{
 public:
  controller_device_event() : event_base{event_type::controller_device_added} {}
  explicit controller_device_event(const SDL_ControllerDeviceEvent& event) noexcept
      : event_base{event}
  {}
  void set_which(const int32 which) noexcept { mEvent.which = which; }
  [[nodiscard]] auto which() const noexcept -> int32 { return mEvent.which; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_ControllerDeviceEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.cdevice = event.get();
  return e;
}
}
namespace cen {
[[nodiscard]] constexpr auto is_little_endian() noexcept -> bool
{
  return SDL_BYTEORDER == SDL_LIL_ENDIAN;
}
[[nodiscard]] constexpr auto is_big_endian() noexcept -> bool
{
  return SDL_BYTEORDER == SDL_BIG_ENDIAN;
}
[[nodiscard]] inline auto swap_byte_order(const uint16 value) noexcept -> uint16
{
  return SDL_Swap16(value);
}
[[nodiscard]] inline auto swap_byte_order(const uint32 value) noexcept -> uint32
{
  return SDL_Swap32(value);
}
[[nodiscard]] inline auto swap_byte_order(const uint64 value) noexcept -> uint64
{
  return SDL_Swap64(value);
}
[[nodiscard]] inline auto swap_byte_order(const float value) noexcept -> float
{
  return SDL_SwapFloat(value);
}
[[nodiscard]] inline auto swap_big_endian(const uint16 value) noexcept -> uint16
{
  return SDL_SwapBE16(value);
}
[[nodiscard]] inline auto swap_big_endian(const uint32 value) noexcept -> uint32
{
  return SDL_SwapBE32(value);
}
[[nodiscard]] inline auto swap_big_endian(const uint64 value) noexcept -> uint64
{
  return SDL_SwapBE64(value);
}
[[nodiscard]] inline auto swap_big_endian(const float value) noexcept -> float
{
  return SDL_SwapFloatBE(value);
}
[[nodiscard]] inline auto swap_little_endian(const uint16 value) noexcept -> uint16
{
  return SDL_SwapLE16(value);
}
[[nodiscard]] inline auto swap_little_endian(const uint32 value) noexcept -> uint32
{
  return SDL_SwapLE32(value);
}
[[nodiscard]] inline auto swap_little_endian(const uint64 value) noexcept -> uint64
{
  return SDL_SwapLE64(value);
}
[[nodiscard]] inline auto swap_little_endian(const float value) noexcept -> float
{
  return SDL_SwapFloatLE(value);
}
}
namespace cen::detail {
template <typename Target, typename Tuple>
class tuple_type_index;
template <typename Target, typename... T>
class tuple_type_index<Target, std::tuple<T...>>
{
 private:
  template <usize... Index>
  [[nodiscard]] constexpr static auto find([[maybe_unused]] std::index_sequence<Index...> seq)
      -> int
  {
    return -1 + ((std::is_same_v<Target, T> ? static_cast<int>(Index) + 1 : 0) + ...);
  }
 public:
  inline constexpr static auto value = find(std::index_sequence_for<T...>{});
};
template <typename Target, typename... T>
inline constexpr int tuple_type_index_v = tuple_type_index<Target, T...>::value;
}
namespace cen {
enum class joy_hat_position : uint8
{
  left_up = SDL_HAT_LEFTUP,
  left = SDL_HAT_LEFT,
  left_down = SDL_HAT_LEFTDOWN,
  up = SDL_HAT_UP,
  centered = SDL_HAT_CENTERED,
  down = SDL_HAT_DOWN,
  right_up = SDL_HAT_RIGHTUP,
  right = SDL_HAT_RIGHT,
  right_down = SDL_HAT_RIGHTDOWN
};
[[nodiscard]] constexpr auto to_string(const joy_hat_position position) -> std::string_view
{
  switch (position) {
    case joy_hat_position::left_up:
      return "left_up";
    case joy_hat_position::left:
      return "left";
    case joy_hat_position::left_down:
      return "left_down";
    case joy_hat_position::up:
      return "up";
    case joy_hat_position::centered:
      return "centered";
    case joy_hat_position::down:
      return "down";
    case joy_hat_position::right_up:
      return "right_up";
    case joy_hat_position::right:
      return "right";
    case joy_hat_position::right_down:
      return "right_down";
    default:
      throw exception{"Did not recognize joystick hat position!"};
  }
}
inline auto operator<<(std::ostream& stream, const joy_hat_position position) -> std::ostream&
{
  return stream << to_string(position);
}
class joy_axis_event final : public event_base<SDL_JoyAxisEvent>
{
 public:
  joy_axis_event() : event_base{event_type::joy_axis_motion} {}
  explicit joy_axis_event(const SDL_JoyAxisEvent& event) noexcept : event_base{event} {}
  void set_which(const SDL_JoystickID which) noexcept { mEvent.which = which; }
  void set_axis(const uint8 axis) noexcept { mEvent.axis = axis; }
  void set_value(const int16 value) noexcept { mEvent.value = value; }
  [[nodiscard]] auto which() const noexcept -> SDL_JoystickID { return mEvent.which; }
  [[nodiscard]] auto axis() const noexcept -> uint8 { return mEvent.axis; }
  [[nodiscard]] auto value() const noexcept -> int16 { return mEvent.value; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_JoyAxisEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.jaxis = event.get();
  return e;
}
class joy_ball_event final : public event_base<SDL_JoyBallEvent>
{
 public:
  joy_ball_event() : event_base{event_type::joy_ball_motion} {}
  explicit joy_ball_event(const SDL_JoyBallEvent& event) noexcept : event_base{event} {}
  void set_which(const SDL_JoystickID which) noexcept { mEvent.which = which; }
  void set_ball(const uint8 ball) noexcept { mEvent.ball = ball; }
  void set_dx(const int16 dx) noexcept { mEvent.xrel = dx; }
  void set_dy(const int16 dy) noexcept { mEvent.yrel = dy; }
  [[nodiscard]] auto which() const noexcept -> SDL_JoystickID { return mEvent.which; }
  [[nodiscard]] auto ball() const noexcept -> uint8 { return mEvent.ball; }
  [[nodiscard]] auto dx() const noexcept -> int16 { return mEvent.xrel; }
  [[nodiscard]] auto dy() const noexcept -> int16 { return mEvent.yrel; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_JoyBallEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.jball = event.get();
  return e;
}
class joy_button_event final : public event_base<SDL_JoyButtonEvent>
{
 public:
  joy_button_event() : event_base{event_type::joy_button_down} {}
  explicit joy_button_event(const SDL_JoyButtonEvent& event) noexcept : event_base{event} {}
  void set_which(const SDL_JoystickID id) noexcept { mEvent.which = id; }
  void set_button(const uint8 index) noexcept { mEvent.button = index; }
  void set_state(const button_state state) noexcept { mEvent.state = to_underlying(state); }
  [[nodiscard]] auto which() const noexcept -> SDL_JoystickID { return mEvent.which; }
  [[nodiscard]] auto button() const noexcept -> uint8 { return mEvent.button; }
  [[nodiscard]] auto state() const noexcept -> button_state
  {
    return static_cast<button_state>(mEvent.state);
  }
  [[nodiscard]] auto is_pressed() const noexcept -> bool
  {
    return state() == button_state::pressed;
  }
  [[nodiscard]] auto is_released() const noexcept -> bool
  {
    return state() == button_state::released;
  }
};
template <>
inline auto as_sdl_event(const event_base<SDL_JoyButtonEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.jbutton = event.get();
  return e;
}
class joy_device_event final : public event_base<SDL_JoyDeviceEvent>
{
 public:
  joy_device_event() : event_base{event_type::joy_device_added} {}
  explicit joy_device_event(const SDL_JoyDeviceEvent& event) noexcept : event_base{event} {}
  void set_which(const int32 which) noexcept { mEvent.which = which; }
  [[nodiscard]] auto which() const noexcept -> int32 { return mEvent.which; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_JoyDeviceEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.jdevice = event.get();
  return e;
}
class joy_hat_event final : public event_base<SDL_JoyHatEvent>
{
 public:
  joy_hat_event() : event_base{event_type::joy_hat_motion} {}
  explicit joy_hat_event(const SDL_JoyHatEvent& event) noexcept : event_base{event} {}
  void set_hat(const uint8 hat) noexcept { mEvent.hat = hat; }
  void set_position(const joy_hat_position value) noexcept
  {
    mEvent.value = to_underlying(value);
  }
  [[nodiscard]] auto hat() const noexcept -> uint8 { return mEvent.hat; }
  [[nodiscard]] auto position() const noexcept -> joy_hat_position
  {
    return static_cast<joy_hat_position>(mEvent.value);
  }
};
template <>
inline auto as_sdl_event(const event_base<SDL_JoyHatEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.jhat = event.get();
  return e;
}
}
namespace cen {
enum class key_mod : uint16
{
  none = KMOD_NONE,
  lshift = KMOD_LSHIFT,
  rshift = KMOD_RSHIFT,
  shift = KMOD_SHIFT,
  lctrl = KMOD_LCTRL,
  rctrl = KMOD_RCTRL,
  ctrl = KMOD_CTRL,
  lalt = KMOD_LALT,
  ralt = KMOD_RALT,
  alt = KMOD_ALT,
  lgui = KMOD_LGUI,
  rgui = KMOD_RGUI,
  gui = KMOD_GUI,
  num = KMOD_NUM,
  caps = KMOD_CAPS,
  mode = KMOD_MODE
};
namespace detail {
[[nodiscard]] inline auto is_active(const key_mod modifiers, const uint16 currentMask) noexcept
    -> bool
{
  if (modifiers == key_mod::none) {
    return !currentMask;
  }
  else {
    return currentMask & to_underlying(modifiers);
  }
}
[[nodiscard]] inline auto is_only_active(const key_mod modifiers,
                                         const uint16 currentMask) noexcept -> bool
{
  if (modifiers == key_mod::none) {
    return !currentMask;
  }
  const auto mask = to_underlying(modifiers);
  const auto hits = currentMask & mask;
  if (hits != mask) {
    return false;
  }
  else {
    const auto others = currentMask & ~hits;
    return hits && !others;
  }
}
[[nodiscard]] inline auto is_only_subset_active(const key_mod modifiers,
                                                const uint16 currentMask) noexcept -> bool
{
  if (modifiers == key_mod::none) {
    return !currentMask;
  }
  const auto mask = to_underlying(modifiers);
  const auto hits = currentMask & mask;
  const auto others = currentMask & ~hits;
  return hits && !others;
}
}
[[nodiscard]] constexpr auto operator~(const key_mod mods) noexcept -> key_mod
{
  return static_cast<key_mod>(~to_underlying(mods));
}
[[nodiscard]] constexpr auto operator&(const key_mod a, const key_mod b) noexcept -> key_mod
{
  return static_cast<key_mod>(to_underlying(a) & to_underlying(b));
}
[[nodiscard]] constexpr auto operator|(const key_mod a, const key_mod b) noexcept -> key_mod
{
  return static_cast<key_mod>(to_underlying(a) | to_underlying(b));
}
[[nodiscard]] inline auto to_string(const key_mod mods) -> std::string
{
  if (mods == key_mod::none) {
    return "none";
  }
  const auto mask = to_underlying(mods);
  std::stringstream stream;
  auto check = [&stream, mask, count = 0](const key_mod mod, const char* name) mutable {
    if (mask & to_underlying(mod)) {
      if (count != 0) {
        stream << ',';
      }
      stream << name;
      ++count;
    }
  };
  check(key_mod::lshift, "lshift");
  check(key_mod::rshift, "rshift");
  check(key_mod::lctrl, "lctrl");
  check(key_mod::rctrl, "rctrl");
  check(key_mod::lalt, "lalt");
  check(key_mod::ralt, "ralt");
  check(key_mod::lgui, "lgui");
  check(key_mod::rgui, "rgui");
  check(key_mod::num, "num");
  check(key_mod::caps, "caps");
  check(key_mod::mode, "mode");
  return stream.str();
}
inline auto operator<<(std::ostream& stream, const key_mod& mods) -> std::ostream&
{
  return stream << to_string(mods);
}
inline void set_modifiers(const key_mod mods) noexcept
{
  SDL_SetModState(static_cast<SDL_Keymod>(mods));
}
[[nodiscard]] inline auto get_modifiers() noexcept -> key_mod
{
  return static_cast<key_mod>(SDL_GetModState());
}
[[nodiscard]] inline auto is_active(const key_mod mods) noexcept -> bool
{
  return detail::is_active(mods, static_cast<uint16>(SDL_GetModState()));
}
[[nodiscard]] inline auto is_only_active(const key_mod mods) noexcept -> bool
{
  return detail::is_only_active(mods, static_cast<uint16>(SDL_GetModState()));
}
[[nodiscard]] inline auto is_only_subset_active(const key_mod mods) noexcept -> bool
{
  return detail::is_only_subset_active(mods, static_cast<uint16>(SDL_GetModState()));
}
class key_code final
{
 public:
  constexpr key_code() noexcept = default;
  constexpr key_code(const key_code&) noexcept = default;
  constexpr key_code(key_code&&) noexcept = default;
  constexpr key_code(const SDL_KeyCode key) noexcept : mKey{key} {}
  explicit key_code(const SDL_Scancode scancode) noexcept
      : mKey{static_cast<SDL_KeyCode>(SDL_GetKeyFromScancode(scancode))}
  {}
  explicit key_code(const char* name) noexcept
      : mKey{static_cast<SDL_KeyCode>(SDL_GetKeyFromName(name))}
  {}
  explicit key_code(const std::string& name) noexcept : key_code{name.c_str()} {}
  constexpr auto operator=(const key_code&) noexcept -> key_code& = default;
  constexpr auto operator=(key_code&&) noexcept -> key_code& = default;
  constexpr auto operator=(const SDL_KeyCode key) noexcept -> key_code&
  {
    mKey = key;
    return *this;
  }
  auto operator=(const SDL_Scancode scancode) noexcept -> key_code&
  {
    mKey = static_cast<SDL_KeyCode>(SDL_GetKeyFromScancode(scancode));
    return *this;
  }
  auto operator=(const char* name) noexcept -> key_code&
  {
    assert(name);
    mKey = static_cast<SDL_KeyCode>(SDL_GetKeyFromName(name));
    return *this;
  }
  auto operator=(const std::string& name) noexcept -> key_code&
  {
    return this->operator=(name.c_str());
  }
  [[nodiscard]] constexpr auto unknown() const noexcept -> bool
  {
    return mKey == SDLK_UNKNOWN;
  }
  [[nodiscard]] auto name() const -> std::string { return SDL_GetKeyName(mKey); }
  [[nodiscard]] auto to_scancode() const noexcept -> SDL_Scancode
  {
    return SDL_GetScancodeFromKey(mKey);
  }
  template <typename Archive>
  void serialize(Archive& archive)
  {
    archive(mKey);
  }
  [[nodiscard]] constexpr auto get() const noexcept -> SDL_KeyCode { return mKey; }
 private:
  SDL_KeyCode mKey{SDLK_UNKNOWN};
};
[[nodiscard]] constexpr auto operator==(const key_code& a, const key_code& b) noexcept -> bool
{
  return a.get() == b.get();
}
[[nodiscard]] constexpr auto operator!=(const key_code& a, const key_code& b) noexcept -> bool
{
  return !(a == b);
}
[[nodiscard]] inline auto to_string(const key_code& code) -> std::string
{
  return "key_code(key: '" + code.name() + "')";
}
inline auto operator<<(std::ostream& stream, const key_code& code) -> std::ostream&
{
  return stream << to_string(code);
}
class scan_code final
{
 public:
  constexpr scan_code() noexcept = default;
  constexpr scan_code(const scan_code&) noexcept = default;
  constexpr scan_code(scan_code&&) noexcept = default;
  constexpr scan_code(const SDL_Scancode scancode) noexcept : mCode{scancode} {}
  explicit scan_code(const SDL_Keycode key) noexcept : mCode{SDL_GetScancodeFromKey(key)} {}
  explicit scan_code(const char* name) noexcept : mCode{SDL_GetScancodeFromName(name)} {}
  explicit scan_code(const std::string& name) noexcept : scan_code{name.c_str()} {}
  constexpr auto operator=(const scan_code&) noexcept -> scan_code& = default;
  constexpr auto operator=(scan_code&&) noexcept -> scan_code& = default;
  constexpr auto operator=(const SDL_Scancode code) noexcept -> scan_code&
  {
    mCode = code;
    return *this;
  }
  auto operator=(const SDL_Keycode keycode) noexcept -> scan_code&
  {
    mCode = SDL_GetScancodeFromKey(keycode);
    return *this;
  }
  auto operator=(const char* name) noexcept -> scan_code&
  {
    assert(name);
    mCode = SDL_GetScancodeFromName(name);
    return *this;
  }
  auto operator=(const std::string& name) noexcept -> scan_code&
  {
    return operator=(name.c_str());
  }
  [[nodiscard]] constexpr static auto count() noexcept -> int { return SDL_NUM_SCANCODES; }
  [[nodiscard]] constexpr auto unknown() const noexcept -> bool
  {
    return mCode == SDL_SCANCODE_UNKNOWN;
  }
  [[nodiscard]] auto name() const -> std::string { return SDL_GetScancodeName(mCode); }
  [[nodiscard]] auto to_key() const noexcept -> SDL_KeyCode
  {
    return static_cast<SDL_KeyCode>(SDL_GetKeyFromScancode(mCode));
  }
  [[nodiscard]] constexpr auto get() const noexcept -> SDL_Scancode { return mCode; }
  template <typename Archive>
  void serialize(Archive& archive)
  {
    archive(mCode);
  }
 private:
  SDL_Scancode mCode{SDL_SCANCODE_UNKNOWN};
};
[[nodiscard]] constexpr auto operator==(const scan_code& a, const scan_code& b) noexcept
    -> bool
{
  return a.get() == b.get();
}
[[nodiscard]] constexpr auto operator!=(const scan_code& a, const scan_code& b) noexcept
    -> bool
{
  return !(a == b);
}
[[nodiscard]] inline auto to_string(const scan_code& code) -> std::string
{
  return "scan_code(key: '" + code.name() + "')";
}
inline auto operator<<(std::ostream& stream, const scan_code& code) -> std::ostream&
{
  return stream << to_string(code);
}
class keyboard final
{
 public:
  keyboard() noexcept { mState = SDL_GetKeyboardState(&mKeyCount); }
  void refresh() { std::copy(mState, mState + mKeyCount, mPrevious.begin()); }
  [[nodiscard]] auto is_pressed(const scan_code& code) const noexcept -> bool
  {
    return check(code, [this](const SDL_Scancode sc) noexcept { return mState[sc]; });
  }
  [[nodiscard]] auto is_pressed(const key_code& code) const noexcept -> bool
  {
    return is_pressed(code.to_scancode());
  }
  [[nodiscard]] auto is_held(const scan_code& code) const noexcept(on_msvc) -> bool
  {
    return check(code, [this](const SDL_Scancode sc) noexcept(on_msvc) {
      return mState[sc] && mPrevious[sc];
    });
  }
  [[nodiscard]] auto is_held(const key_code& code) const noexcept(on_msvc) -> bool
  {
    return is_held(code.to_scancode());
  }
  [[nodiscard]] auto just_pressed(const scan_code& code) const noexcept(on_msvc) -> bool
  {
    return check(code, [this](const SDL_Scancode sc) noexcept(on_msvc) {
      return mState[sc] && !mPrevious[sc];
    });
  }
  [[nodiscard]] auto just_pressed(const key_code& code) const noexcept(on_msvc) -> bool
  {
    return just_pressed(code.to_scancode());
  }
  [[nodiscard]] auto just_released(const scan_code& code) const noexcept(on_msvc) -> bool
  {
    return check(code, [this](const SDL_Scancode sc) noexcept(on_msvc) {
      return !mState[sc] && mPrevious[sc];
    });
  }
  [[nodiscard]] auto just_released(const key_code& code) const noexcept(on_msvc) -> bool
  {
    return just_released(code.to_scancode());
  }
  [[nodiscard]] auto size() const noexcept -> int { return mKeyCount; }
 private:
  const uint8* mState{};
  std::array<uint8, cen::scan_code::count()> mPrevious{};
  int mKeyCount{};
  template <typename Predicate>
  auto check(const cen::scan_code& code, Predicate&& predicate) const
      noexcept(noexcept(predicate(code.get()))) -> bool
  {
    const auto sc = code.get();
    if (sc >= 0 && sc < mKeyCount) {
      return predicate(sc);
    }
    else {
      return false;
    }
  }
};
[[nodiscard]] inline auto has_screen_keyboard() noexcept -> bool
{
  return SDL_HasScreenKeyboardSupport() == SDL_TRUE;
}
[[nodiscard]] inline auto to_string(const keyboard& keyboard) -> std::string
{
  return "keyboard(size: " + std::to_string(keyboard.size()) + ")";
}
inline auto operator<<(std::ostream& stream, const keyboard& keyboard) -> std::ostream&
{
  return stream << to_string(keyboard);
}
namespace keycodes {
inline constexpr key_code unknown;
inline constexpr key_code a{SDLK_a};
inline constexpr key_code b{SDLK_b};
inline constexpr key_code c{SDLK_c};
inline constexpr key_code d{SDLK_d};
inline constexpr key_code e{SDLK_e};
inline constexpr key_code f{SDLK_f};
inline constexpr key_code g{SDLK_g};
inline constexpr key_code h{SDLK_h};
inline constexpr key_code i{SDLK_i};
inline constexpr key_code j{SDLK_j};
inline constexpr key_code k{SDLK_k};
inline constexpr key_code l{SDLK_l};
inline constexpr key_code m{SDLK_m};
inline constexpr key_code n{SDLK_n};
inline constexpr key_code o{SDLK_o};
inline constexpr key_code p{SDLK_p};
inline constexpr key_code q{SDLK_q};
inline constexpr key_code r{SDLK_r};
inline constexpr key_code s{SDLK_s};
inline constexpr key_code t{SDLK_t};
inline constexpr key_code u{SDLK_u};
inline constexpr key_code v{SDLK_v};
inline constexpr key_code w{SDLK_w};
inline constexpr key_code x{SDLK_x};
inline constexpr key_code y{SDLK_y};
inline constexpr key_code z{SDLK_z};
inline constexpr key_code one{SDLK_1};
inline constexpr key_code two{SDLK_2};
inline constexpr key_code three{SDLK_3};
inline constexpr key_code four{SDLK_4};
inline constexpr key_code five{SDLK_5};
inline constexpr key_code six{SDLK_6};
inline constexpr key_code seven{SDLK_7};
inline constexpr key_code eight{SDLK_8};
inline constexpr key_code nine{SDLK_9};
inline constexpr key_code zero{SDLK_0};
inline constexpr key_code f1{SDLK_F1};
inline constexpr key_code f2{SDLK_F2};
inline constexpr key_code f3{SDLK_F3};
inline constexpr key_code f4{SDLK_F4};
inline constexpr key_code f5{SDLK_F5};
inline constexpr key_code f6{SDLK_F6};
inline constexpr key_code f7{SDLK_F7};
inline constexpr key_code f8{SDLK_F8};
inline constexpr key_code f9{SDLK_F9};
inline constexpr key_code f10{SDLK_F10};
inline constexpr key_code f11{SDLK_F11};
inline constexpr key_code f12{SDLK_F12};
inline constexpr key_code left{SDLK_LEFT};
inline constexpr key_code right{SDLK_RIGHT};
inline constexpr key_code up{SDLK_UP};
inline constexpr key_code down{SDLK_DOWN};
inline constexpr key_code space{SDLK_SPACE};
inline constexpr key_code enter{SDLK_RETURN};
inline constexpr key_code escape{SDLK_ESCAPE};
inline constexpr key_code backspace{SDLK_BACKSPACE};
inline constexpr key_code tab{SDLK_TAB};
inline constexpr key_code caps_lock{SDLK_CAPSLOCK};
inline constexpr key_code left_shift{SDLK_LSHIFT};
inline constexpr key_code right_shift{SDLK_RSHIFT};
inline constexpr key_code left_ctrl{SDLK_LCTRL};
inline constexpr key_code right_ctrl{SDLK_RCTRL};
inline constexpr key_code left_alt{SDLK_LALT};
inline constexpr key_code right_alt{SDLK_RALT};
inline constexpr key_code left_gui{SDLK_LGUI};
inline constexpr key_code right_gui{SDLK_RGUI};
}
namespace scancodes {
inline constexpr scan_code unknown;
inline constexpr scan_code a{SDL_SCANCODE_A};
inline constexpr scan_code b{SDL_SCANCODE_B};
inline constexpr scan_code c{SDL_SCANCODE_C};
inline constexpr scan_code d{SDL_SCANCODE_D};
inline constexpr scan_code e{SDL_SCANCODE_E};
inline constexpr scan_code f{SDL_SCANCODE_F};
inline constexpr scan_code g{SDL_SCANCODE_G};
inline constexpr scan_code h{SDL_SCANCODE_H};
inline constexpr scan_code i{SDL_SCANCODE_I};
inline constexpr scan_code j{SDL_SCANCODE_J};
inline constexpr scan_code k{SDL_SCANCODE_K};
inline constexpr scan_code l{SDL_SCANCODE_L};
inline constexpr scan_code m{SDL_SCANCODE_M};
inline constexpr scan_code n{SDL_SCANCODE_N};
inline constexpr scan_code o{SDL_SCANCODE_O};
inline constexpr scan_code p{SDL_SCANCODE_P};
inline constexpr scan_code q{SDL_SCANCODE_Q};
inline constexpr scan_code r{SDL_SCANCODE_R};
inline constexpr scan_code s{SDL_SCANCODE_S};
inline constexpr scan_code t{SDL_SCANCODE_T};
inline constexpr scan_code u{SDL_SCANCODE_U};
inline constexpr scan_code v{SDL_SCANCODE_V};
inline constexpr scan_code w{SDL_SCANCODE_W};
inline constexpr scan_code x{SDL_SCANCODE_X};
inline constexpr scan_code y{SDL_SCANCODE_Y};
inline constexpr scan_code z{SDL_SCANCODE_Z};
inline constexpr scan_code one{SDL_SCANCODE_1};
inline constexpr scan_code two{SDL_SCANCODE_2};
inline constexpr scan_code three{SDL_SCANCODE_3};
inline constexpr scan_code four{SDL_SCANCODE_4};
inline constexpr scan_code five{SDL_SCANCODE_5};
inline constexpr scan_code six{SDL_SCANCODE_6};
inline constexpr scan_code seven{SDL_SCANCODE_7};
inline constexpr scan_code eight{SDL_SCANCODE_8};
inline constexpr scan_code nine{SDL_SCANCODE_9};
inline constexpr scan_code zero{SDL_SCANCODE_0};
inline constexpr scan_code f1{SDL_SCANCODE_F1};
inline constexpr scan_code f2{SDL_SCANCODE_F2};
inline constexpr scan_code f3{SDL_SCANCODE_F3};
inline constexpr scan_code f4{SDL_SCANCODE_F4};
inline constexpr scan_code f5{SDL_SCANCODE_F5};
inline constexpr scan_code f6{SDL_SCANCODE_F6};
inline constexpr scan_code f7{SDL_SCANCODE_F7};
inline constexpr scan_code f8{SDL_SCANCODE_F8};
inline constexpr scan_code f9{SDL_SCANCODE_F9};
inline constexpr scan_code f10{SDL_SCANCODE_F10};
inline constexpr scan_code f11{SDL_SCANCODE_F11};
inline constexpr scan_code f12{SDL_SCANCODE_F12};
inline constexpr scan_code left{SDL_SCANCODE_LEFT};
inline constexpr scan_code right{SDL_SCANCODE_RIGHT};
inline constexpr scan_code up{SDL_SCANCODE_UP};
inline constexpr scan_code down{SDL_SCANCODE_DOWN};
inline constexpr scan_code space{SDL_SCANCODE_SPACE};
inline constexpr scan_code enter{SDL_SCANCODE_RETURN};
inline constexpr scan_code escape{SDL_SCANCODE_ESCAPE};
inline constexpr scan_code backspace{SDL_SCANCODE_BACKSPACE};
inline constexpr scan_code tab{SDL_SCANCODE_TAB};
inline constexpr scan_code caps_lock{SDL_SCANCODE_CAPSLOCK};
inline constexpr scan_code left_shift{SDL_SCANCODE_LSHIFT};
inline constexpr scan_code right_shift{SDL_SCANCODE_RSHIFT};
inline constexpr scan_code left_ctrl{SDL_SCANCODE_LCTRL};
inline constexpr scan_code right_ctrl{SDL_SCANCODE_RCTRL};
inline constexpr scan_code left_alt{SDL_SCANCODE_LALT};
inline constexpr scan_code right_alt{SDL_SCANCODE_RALT};
inline constexpr scan_code left_gui{SDL_SCANCODE_LGUI};
inline constexpr scan_code right_gui{SDL_SCANCODE_RGUI};
}
}
namespace cen {
enum class texture_access
{
  non_lockable = SDL_TEXTUREACCESS_STATIC,
  streaming = SDL_TEXTUREACCESS_STREAMING,
  target = SDL_TEXTUREACCESS_TARGET
};
[[nodiscard]] constexpr auto to_string(const texture_access access) -> std::string_view
{
  switch (access) {
    case texture_access::non_lockable:
      return "non_lockable";
    case texture_access::streaming:
      return "streaming";
    case texture_access::target:
      return "target";
    default:
      throw exception{"Did not recognize texture access!"};
  }
}
inline auto operator<<(std::ostream& stream, const texture_access access) -> std::ostream&
{
  return stream << to_string(access);
}
template <typename T>
class basic_texture;
using texture = basic_texture<detail::owner_tag>;
using texture_handle = basic_texture<detail::handle_tag>;
template <typename T>
class basic_texture final
{
 public:
  explicit basic_texture(maybe_owner<SDL_Texture*> source) noexcept(detail::is_handle<T>)
      : mTexture{source}
  {
    if constexpr (detail::is_owner<T>) {
      if (!mTexture) {
        throw exception{"Cannot create texture from null pointer!"};
      }
    }
  }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit basic_texture(texture& owner) noexcept : mTexture{owner.get()}
  {}
  void set_alpha_mod(const uint8 alpha) noexcept { SDL_SetTextureAlphaMod(mTexture, alpha); }
  void set_color_mod(const color& color) noexcept
  {
    SDL_SetTextureColorMod(mTexture, color.red(), color.green(), color.blue());
  }
  void set_blend_mode(const blend_mode mode) noexcept
  {
    SDL_SetTextureBlendMode(mTexture, static_cast<SDL_BlendMode>(mode));
  }
  [[nodiscard]] auto size() const noexcept -> iarea
  {
    int width{};
    int height{};
    SDL_QueryTexture(mTexture, nullptr, nullptr, &width, &height);
    return {width, height};
  }
  [[nodiscard]] auto width() const noexcept -> int
  {
    const auto [width, height] = size();
    return width;
  }
  [[nodiscard]] auto height() const noexcept -> int
  {
    const auto [width, height] = size();
    return height;
  }
  [[nodiscard]] auto format() const noexcept -> pixel_format
  {
    uint32 format{};
    SDL_QueryTexture(mTexture, &format, nullptr, nullptr, nullptr);
    return static_cast<pixel_format>(format);
  }
  [[nodiscard]] auto access() const noexcept -> texture_access
  {
    int access{};
    SDL_QueryTexture(mTexture, nullptr, &access, nullptr, nullptr);
    return static_cast<texture_access>(access);
  }
  [[nodiscard]] auto is_static() const noexcept -> bool
  {
    return access() == texture_access::non_lockable;
  }
  [[nodiscard]] auto is_target() const noexcept -> bool
  {
    return access() == texture_access::target;
  }
  [[nodiscard]] auto is_streaming() const noexcept -> bool
  {
    return access() == texture_access::streaming;
  }
  [[nodiscard]] auto alpha_mod() const noexcept -> uint8
  {
    uint8 alpha{};
    SDL_GetTextureAlphaMod(mTexture, &alpha);
    return alpha;
  }
  [[nodiscard]] auto color_mod() const noexcept -> color
  {
    uint8 red{};
    uint8 green{};
    uint8 blue{};
    SDL_GetTextureColorMod(mTexture, &red, &green, &blue);
    return {red, green, blue};
  }
  [[nodiscard]] auto get_blend_mode() const noexcept -> blend_mode
  {
    SDL_BlendMode mode{};
    SDL_GetTextureBlendMode(mTexture, &mode);
    return static_cast<blend_mode>(mode);
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] auto release() noexcept -> owner<SDL_Texture*>
  {
    return mTexture.release();
  }
  [[nodiscard]] auto get() const noexcept -> SDL_Texture* { return mTexture.get(); }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit operator bool() const noexcept
  {
    return mTexture != nullptr;
  }
 private:
  detail::pointer<T, SDL_Texture> mTexture;
};
template <typename T>
[[nodiscard]] auto to_string(const basic_texture<T>& texture) -> std::string
{
  return "texture(data: " + detail::address_of(texture.get()) +
         ", width: " + std::to_string(texture.width()) +
         ", height: " + std::to_string(texture.height()) + ")";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_texture<T>& texture) -> std::ostream&
{
  return stream << to_string(texture);
}
}
namespace cen {
class unicode_string final
{
 public:
  using value_type = std::vector<unicode_t>::value_type;
  using pointer = std::vector<unicode_t>::pointer;
  using const_pointer = std::vector<unicode_t>::const_pointer;
  using reference = std::vector<unicode_t>::reference;
  using const_reference = std::vector<unicode_t>::const_reference;
  using iterator = std::vector<unicode_t>::iterator;
  using const_iterator = std::vector<unicode_t>::const_iterator;
  using size_type = std::vector<unicode_t>::size_type;
  unicode_string() { mData.push_back(0); }
                 unicode_string(const std::u16string_view str)
  {
    reserve(str.size());
    for (const auto ch : str) {
      mData.push_back(ch);
    }
    mData.push_back(0);
  }
  unicode_string(std::initializer_list<unicode_t> codes)
  {
    reserve(codes.size() + 1);
    mData.insert(mData.end(), codes.begin(), codes.end());
    mData.push_back(0);
  }
  void reserve(const size_type n) { mData.reserve(n + 1u); }
  void append(const unicode_t ch) { mData.insert(mData.end() - 1, ch); }
  template <typename... Character>
  void append(Character... code)
  {
    static_assert(sizeof...(Character) != 0, "Must append at least 1 value!");
    static_assert((std::is_convertible_v<Character, unicode_t> && ...),
                  "Must append unicode_t values!");
    (append(static_cast<unicode_t>(code)), ...);
  }
  void operator+=(const unicode_t ch) { append(ch); }
  void pop_back()
  {
    if (!empty()) {
      mData.erase(mData.end() - 2);
    }
  }
  [[nodiscard]] auto at(const size_type index) -> reference
  {
    if (is_valid_index(index)) {
      return mData[index];
    }
    else {
      throw exception{"Invalid unicode string index!"};
    }
  }
  [[nodiscard]] auto at(const size_type index) const -> const_reference
  {
    if (is_valid_index(index)) {
      return mData[index];
    }
    else {
      throw exception{"Invalid unicode string index!"};
    }
  }
  [[nodiscard]] auto operator[](const size_type index) noexcept(on_msvc) -> reference
  {
    assert(is_valid_index(index));
    return mData[index];
  }
  [[nodiscard]] auto operator[](const size_type index) const noexcept(on_msvc)
      -> const_reference
  {
    assert(is_valid_index(index));
    return mData[index];
  }
  [[nodiscard]] auto size() const noexcept -> size_type
  {
    assert(!mData.empty());
    return mData.size() - 1u;
  }
  [[nodiscard]] auto capacity() const noexcept -> size_type
  {
    assert(mData.capacity() >= 1u);
    return mData.capacity() - 1u;
  }
  [[nodiscard]] auto empty() const noexcept -> bool { return mData.size() == 1; }
  [[nodiscard]] auto data() noexcept -> pointer { return mData.data(); }
  [[nodiscard]] auto data() const noexcept -> const_pointer { return mData.data(); }
  [[nodiscard]] auto begin() noexcept -> iterator { return mData.begin(); }
  [[nodiscard]] auto begin() const noexcept -> const_iterator { return mData.begin(); }
  [[nodiscard]] auto end() noexcept -> iterator { return mData.end() - 1; }
  [[nodiscard]] auto end() const noexcept -> const_iterator { return mData.end() - 1; }
  template <typename Archive>
  void serialize(Archive& archive)
  {
    archive(mData);
  }
 private:
  std::vector<unicode_t> mData;
  [[nodiscard]] auto is_valid_index(const size_type index) const noexcept -> bool
  {
    return index < mData.size() - 1;
  }
};
[[nodiscard]] inline auto operator==(const unicode_string& a, const unicode_string& b) -> bool
{
  if (a.size() != b.size()) {
    return false;
  }
  for (unicode_string::size_type index = 0; index < a.size(); ++index) {
    const auto aChar = a.at(index);
    const auto bChar = b.at(index);
    if (aChar != bChar) {
      return false;
    }
  }
  return true;
}
[[nodiscard]] inline auto operator!=(const unicode_string& a, const unicode_string& b) -> bool
{
  return !(a == b);
}
}
namespace cen {
enum class renderer_flip
{
  none = SDL_FLIP_NONE,
  horizontal = SDL_FLIP_HORIZONTAL,
  vertical = SDL_FLIP_VERTICAL
};
[[nodiscard]] constexpr auto to_string(const renderer_flip flip) -> std::string_view
{
  switch (flip) {
    case renderer_flip::none:
      return "none";
    case renderer_flip::horizontal:
      return "horizontal";
    case renderer_flip::vertical:
      return "vertical";
    default:
      throw exception{"Did not recognize renderer flip value!"};
  }
}
inline auto operator<<(std::ostream& stream, const renderer_flip flip) -> std::ostream&
{
  return stream << to_string(flip);
}
struct renderer_scale final
{
  float x{};
  float y{};
};
template <typename T>
class basic_renderer;
using renderer = basic_renderer<detail::owner_tag>;
using renderer_handle = basic_renderer<detail::handle_tag>;
template <typename T>
class basic_renderer final
{
 public:
  enum renderer_flags : uint32
  {
    software = SDL_RENDERER_SOFTWARE,
    accelerated = SDL_RENDERER_ACCELERATED,
    target_textures = SDL_RENDERER_TARGETTEXTURE,
    vsync = SDL_RENDERER_PRESENTVSYNC
  };
  explicit basic_renderer(maybe_owner<SDL_Renderer*> renderer) noexcept(detail::is_handle<T>)
      : mRenderer{renderer}
  {
    if constexpr (detail::is_owner<T>) {
      if (!get()) {
        throw exception{"Cannot create renderer from null pointer!"};
      }
    }
  }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit basic_renderer(const renderer& owner) noexcept : mRenderer{owner.get()}
  {}
  template <typename X>
  [[nodiscard]] auto make_texture(const basic_surface<X>& surface) const -> texture
  {
    if (auto* ptr = SDL_CreateTextureFromSurface(get(), surface.get())) {
      return texture{ptr};
    }
    else {
      throw sdl_error{};
    }
  }
  [[nodiscard]] auto make_texture(const iarea& size,
                                  const pixel_format format,
                                  const texture_access access) const -> texture
  {
    if (auto* ptr = SDL_CreateTexture(get(),
                                      to_underlying(format),
                                      to_underlying(access),
                                      size.width,
                                      size.height)) {
      return texture{ptr};
    }
    else {
      throw sdl_error{};
    }
  }
  [[nodiscard]] auto make_texture(const char* path) const -> texture
  {
    assert(path);
    if (auto* ptr = IMG_LoadTexture(get(), path)) {
      return texture{ptr};
    }
    else {
      throw img_error{};
    }
  }
  [[nodiscard]] auto make_texture(const std::string& path) const -> texture
  {
    return make_texture(path.c_str());
  }
  auto clear() noexcept -> result { return SDL_RenderClear(get()) == 0; }
  void clear_with(const color& color) noexcept
  {
    const auto previous = get_color();
    set_color(color);
    clear();
    set_color(previous);
  }
  void present() noexcept { SDL_RenderPresent(get()); }
  void fill() noexcept
  {
    cen::irect rect;
    rect.set_size(output_size());
    fill_rect(rect);
  }
  void fill_with(const color& color) noexcept
  {
    const auto previous = get_color();
    set_color(color);
    fill();
    set_color(previous);
  }
  template <typename X>
  auto draw_rect(const basic_rect<X>& rect) noexcept -> result
  {
    if constexpr (basic_rect<X>::integral) {
      return SDL_RenderDrawRect(get(), rect.data()) == 0;
    }
    else {
      return SDL_RenderDrawRectF(get(), rect.data()) == 0;
    }
  }
  template <typename X>
  auto fill_rect(const basic_rect<X>& rect) noexcept -> result
  {
    if constexpr (basic_rect<X>::integral) {
      return SDL_RenderFillRect(get(), rect.data()) == 0;
    }
    else {
      return SDL_RenderFillRectF(get(), rect.data()) == 0;
    }
  }
  template <typename X>
  auto draw_line(const basic_point<X>& start, const basic_point<X>& end) noexcept -> result
  {
    if constexpr (basic_point<X>::integral) {
      return SDL_RenderDrawLine(get(), start.x(), start.y(), end.x(), end.y()) == 0;
    }
    else {
      return SDL_RenderDrawLineF(get(), start.x(), start.y(), end.x(), end.y()) == 0;
    }
  }
  template <typename Container>
  auto draw_lines(const Container& container) noexcept -> result
  {
    using point_t = typename Container::value_type;
    using value_t = typename point_t::value_type;
    if (!container.empty()) {
      const auto& front = container.front();
      const auto* first = front.data();
      if constexpr (std::is_same_v<value_t, int>) {
        return SDL_RenderDrawLines(get(), first, isize(container)) == 0;
      }
      else {
        return SDL_RenderDrawLinesF(get(), first, isize(container)) == 0;
      }
    }
    else {
      return failure;
    }
  }
  template <typename X>
  auto draw_point(const basic_point<X>& point) noexcept -> result
  {
    if constexpr (basic_point<X>::integral) {
      return SDL_RenderDrawPoint(get(), point.x(), point.y()) == 0;
    }
    else {
      return SDL_RenderDrawPointF(get(), point.x(), point.y()) == 0;
    }
  }
  template <typename X>
  void draw_circle(const basic_point<X>& position, const float radius) noexcept
  {
    using value_t = typename basic_point<X>::value_type;
    auto error = -radius;
    auto x = radius - 0.5f;
    auto y = 0.5f;
    const auto cx = static_cast<float>(position.x()) - 0.5f;
    const auto cy = static_cast<float>(position.y()) - 0.5f;
    while (x >= y) {
      draw_point<value_t>({static_cast<value_t>(cx + x), static_cast<value_t>(cy + y)});
      draw_point<value_t>({static_cast<value_t>(cx + y), static_cast<value_t>(cy + x)});
      if (x != 0) {
        draw_point<value_t>({static_cast<value_t>(cx - x), static_cast<value_t>(cy + y)});
        draw_point<value_t>({static_cast<value_t>(cx + y), static_cast<value_t>(cy - x)});
      }
      if (y != 0) {
        draw_point<value_t>({static_cast<value_t>(cx + x), static_cast<value_t>(cy - y)});
        draw_point<value_t>({static_cast<value_t>(cx - y), static_cast<value_t>(cy + x)});
      }
      if (x != 0 && y != 0) {
        draw_point<value_t>({static_cast<value_t>(cx - x), static_cast<value_t>(cy - y)});
        draw_point<value_t>({static_cast<value_t>(cx - y), static_cast<value_t>(cy - x)});
      }
      error += y;
      ++y;
      error += y;
      if (error >= 0) {
        --x;
        error -= x;
        error -= x;
      }
    }
  }
  template <typename X>
  void fill_circle(const basic_point<X>& center, const float radius)
  {
    const auto cx = static_cast<float>(center.x());
    const auto cy = static_cast<float>(center.y());
    for (auto dy = 1.0f; dy <= radius; dy += 1.0f) {
      const auto dx = std::floor(std::sqrt((2.0f * radius * dy) - (dy * dy)));
      draw_line<float>({cx - dx, cy + dy - radius}, {cx + dx, cy + dy - radius});
      draw_line<float>({cx - dx, cy - dy + radius}, {cx + dx, cy - dy + radius});
    }
  }
  template <typename X, typename Y>
  auto render(const basic_texture<X>& texture, const basic_point<Y>& pos) noexcept -> result
  {
    if constexpr (basic_point<Y>::floating) {
      const auto size = texture.size().as_f();
      const SDL_FRect dst{pos.x(), pos.y(), size.width, size.height};
      return SDL_RenderCopyF(get(), texture.get(), nullptr, &dst) == 0;
    }
    else {
      const SDL_Rect dst{pos.x(), pos.y(), texture.width(), texture.height()};
      return SDL_RenderCopy(get(), texture.get(), nullptr, &dst) == 0;
    }
  }
  template <typename X, typename Y>
  auto render(const basic_texture<X>& texture, const basic_rect<Y>& dst) noexcept -> result
  {
    if constexpr (basic_rect<Y>::floating) {
      return SDL_RenderCopyF(get(), texture.get(), nullptr, dst.data()) == 0;
    }
    else {
      return SDL_RenderCopy(get(), texture.get(), nullptr, dst.data()) == 0;
    }
  }
  template <typename X, typename Y>
  auto render(const basic_texture<X>& texture,
              const irect& src,
              const basic_rect<Y>& dst) noexcept -> result
  {
    if constexpr (basic_rect<Y>::floating) {
      return SDL_RenderCopyF(get(), texture.get(), src.data(), dst.data()) == 0;
    }
    else {
      return SDL_RenderCopy(get(), texture.get(), src.data(), dst.data()) == 0;
    }
  }
  template <typename X, typename Y>
  auto render(const basic_texture<X>& texture,
              const irect& src,
              const basic_rect<Y>& dst,
              const double angle) noexcept -> result
  {
    if constexpr (basic_rect<Y>::floating) {
      return SDL_RenderCopyExF(get(),
                               texture.get(),
                               src.data(),
                               dst.data(),
                               angle,
                               nullptr,
                               SDL_FLIP_NONE) == 0;
    }
    else {
      return SDL_RenderCopyEx(get(),
                              texture.get(),
                              src.data(),
                              dst.data(),
                              angle,
                              nullptr,
                              SDL_FLIP_NONE) == 0;
    }
  }
  template <typename X, typename Y, typename Z>
  auto render(const basic_texture<X>& texture,
              const irect& src,
              const basic_rect<Y>& dst,
              const double angle,
              const basic_point<Z>& center,
              const renderer_flip flip) noexcept -> result
  {
    static_assert(std::is_same_v<typename basic_rect<Y>::value_type,
                                 typename basic_point<Z>::value_type>,
                  "Destination rectangle and center point must have the same "
                  "value types (int or float)!");
    if constexpr (basic_rect<Y>::floating) {
      return SDL_RenderCopyExF(get(),
                               texture.get(),
                               src.data(),
                               dst.data(),
                               angle,
                               center.data(),
                               static_cast<SDL_RendererFlip>(flip)) == 0;
    }
    else {
      return SDL_RenderCopyEx(get(),
                              texture.get(),
                              src.data(),
                              dst.data(),
                              angle,
                              center.data(),
                              static_cast<SDL_RendererFlip>(flip)) == 0;
    }
  }
  auto reset_target() noexcept -> result { return SDL_SetRenderTarget(get(), nullptr) == 0; }
  template <typename X>
  auto set_target(basic_texture<X>& target) noexcept -> result
  {
    assert(target.is_target());
    return SDL_SetRenderTarget(get(), target.get()) == 0;
  }
  [[nodiscard]] auto get_target() noexcept -> texture_handle
  {
    return texture_handle{SDL_GetRenderTarget(get())};
  }
  auto reset_clip() noexcept -> result { return SDL_RenderSetClipRect(get(), nullptr) == 0; }
  auto set_clip(const irect& area) noexcept -> result
  {
    return SDL_RenderSetClipRect(get(), area.data()) == 0;
  }
  [[nodiscard]] auto clip() const noexcept -> maybe<irect>
  {
    irect rect;
    SDL_RenderGetClipRect(get(), rect.data());
    if (rect.has_area()) {
      return rect;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto is_clipping_enabled() const noexcept -> bool
  {
    return SDL_RenderIsClipEnabled(get());
  }
  auto set_logical_size(const iarea size) noexcept -> result
  {
    assert(size.width >= 0);
    assert(size.height >= 0);
    return SDL_RenderSetLogicalSize(get(), size.width, size.height) == 0;
  }
  auto set_logical_integer_scaling(const bool enabled) noexcept -> result
  {
    return SDL_RenderSetIntegerScale(get(), enabled ? SDL_TRUE : SDL_FALSE) == 0;
  }
  [[nodiscard]] auto logical_size() const noexcept -> iarea
  {
    iarea size{};
    SDL_RenderGetLogicalSize(get(), &size.width, &size.height);
    return size;
  }
  [[nodiscard]] auto using_integer_logical_scaling() const noexcept -> bool
  {
    return SDL_RenderGetIntegerScale(get());
  }
  auto set_color(const color& color) noexcept -> result
  {
    return SDL_SetRenderDrawColor(get(),
                                  color.red(),
                                  color.green(),
                                  color.blue(),
                                  color.alpha()) == 0;
  }
  auto set_blend_mode(const blend_mode mode) noexcept -> result
  {
    return SDL_SetRenderDrawBlendMode(get(), static_cast<SDL_BlendMode>(mode)) == 0;
  }
  auto set_viewport(const irect& viewport) noexcept -> result
  {
    return SDL_RenderSetViewport(get(), viewport.data()) == 0;
  }
  auto set_scale(const renderer_scale scale) noexcept -> result
  {
    assert(scale.x > 0);
    assert(scale.y > 0);
    return SDL_RenderSetScale(get(), scale.x, scale.y) == 0;
  }
  [[nodiscard]] auto get_color() const noexcept -> color
  {
    uint8 red{};
    uint8 green{};
    uint8 blue{};
    uint8 alpha{};
    SDL_GetRenderDrawColor(get(), &red, &green, &blue, &alpha);
    return {red, green, blue, alpha};
  }
  [[nodiscard]] auto get_blend_mode() const noexcept -> blend_mode
  {
    SDL_BlendMode mode{};
    SDL_GetRenderDrawBlendMode(get(), &mode);
    return static_cast<blend_mode>(mode);
  }
  [[nodiscard]] auto viewport() const noexcept -> irect
  {
    irect viewport{};
    SDL_RenderGetViewport(get(), viewport.data());
    return viewport;
  }
  [[nodiscard]] auto scale() const noexcept -> renderer_scale
  {
    renderer_scale scale;
    SDL_RenderGetScale(get(), &scale.x, &scale.y);
    return scale;
  }
  [[nodiscard]] auto capture(const pixel_format format) const -> surface
  {
    surface image{output_size(), format};
    if (!image.lock()) {
      throw sdl_error{};
    }
    if (const auto res =
            SDL_RenderReadPixels(get(), nullptr, 0, image.pixel_data(), image.pitch());
        res == -1) {
      throw sdl_error{};
    }
    image.unlock();
    return image;
  }
  [[nodiscard]] auto output_size() const noexcept -> iarea
  {
    iarea size{};
    SDL_GetRendererOutputSize(get(), &size.width, &size.height);
    return size;
  }
  [[nodiscard]] auto get() const noexcept -> SDL_Renderer* { return mRenderer.get(); }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit operator bool() const noexcept
  {
    return mRenderer != nullptr;
  }
  [[nodiscard]] constexpr static auto default_flags() noexcept -> uint32
  {
    return accelerated | vsync;
  }
 private:
  detail::pointer<T, SDL_Renderer> mRenderer;
};
template <typename T>
[[nodiscard]] auto to_string(const basic_renderer<T>& renderer) -> std::string
{
  return "renderer(data: " + detail::address_of(renderer.get()) + ")";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_renderer<T>& renderer) -> std::ostream&
{
  return stream << to_string(renderer);
}
class renderer_info final
{
  template <typename T>
  friend auto get_info(const basic_renderer<T>& renderer) noexcept -> maybe<renderer_info>;
 public:
  using size_type = usize;
  [[nodiscard]] auto supported_flags() const noexcept -> uint32 { return mInfo.flags; }
  [[nodiscard]] auto has_vsync() const noexcept -> bool
  {
    return supported_flags() & SDL_RENDERER_PRESENTVSYNC;
  }
  [[nodiscard]] auto has_target_textures() const noexcept -> bool
  {
    return supported_flags() & SDL_RENDERER_TARGETTEXTURE;
  }
  [[nodiscard]] auto is_accelerated() const noexcept -> bool
  {
    return supported_flags() & SDL_RENDERER_ACCELERATED;
  }
  [[nodiscard]] auto is_software() const noexcept -> bool
  {
    return supported_flags() & SDL_RENDERER_SOFTWARE;
  }
  [[nodiscard]] auto format_count() const noexcept -> size_type
  {
    return static_cast<size_type>(mInfo.num_texture_formats);
  }
  [[nodiscard]] auto get_format(const size_type index) const -> pixel_format
  {
    if (index < format_count()) {
      return static_cast<pixel_format>(mInfo.texture_formats[index]);
    }
    else {
      throw exception{"Invalid pixel format index!"};
    }
  }
  [[nodiscard]] auto max_texture_size() const noexcept -> iarea
  {
    return {mInfo.max_texture_width, mInfo.max_texture_height};
  }
  [[nodiscard]] auto max_texture_width() const noexcept -> int
  {
    return mInfo.max_texture_width;
  }
  [[nodiscard]] auto max_texture_height() const noexcept -> int
  {
    return mInfo.max_texture_height;
  }
  [[nodiscard]] auto name() const noexcept -> const char* { return mInfo.name; }
 private:
  SDL_RendererInfo mInfo;
  explicit renderer_info(const SDL_RendererInfo info) noexcept : mInfo{info} {}
};
[[nodiscard]] inline auto to_string(const renderer_info& info) -> std::string
{
  using namespace std::string_literals;
  return "renderer_info(name: '"s + str_or_na(info.name()) + "')";
}
inline auto operator<<(std::ostream& stream, const renderer_info& info) -> std::ostream&
{
  return stream << to_string(info);
}
template <typename T>
[[nodiscard]] auto get_info(const basic_renderer<T>& renderer) noexcept -> maybe<renderer_info>
{
  SDL_RendererInfo info{};
  if (SDL_GetRendererInfo(renderer.get(), &info) == 0) {
    return renderer_info{info};
  }
  else {
    return nothing;
  }
}
[[nodiscard]] inline auto render_driver_count() noexcept -> int
{
  return SDL_GetNumRenderDrivers();
}
[[nodiscard]] inline auto video_driver_count() noexcept -> int
{
  return SDL_GetNumVideoDrivers();
}
}
namespace cen {
template <typename T>
class basic_window;
using window = basic_window<detail::owner_tag>;
using window_handle = basic_window<detail::handle_tag>;
template <typename T>
class basic_window final
{
 public:
  enum window_flags : uint32
  {
    fullscreen = SDL_WINDOW_FULLSCREEN,
    fullscreen_desktop = SDL_WINDOW_FULLSCREEN_DESKTOP,
    shown = SDL_WINDOW_SHOWN,
    hidden = SDL_WINDOW_HIDDEN,
    minimized = SDL_WINDOW_MINIMIZED,
    maximized = SDL_WINDOW_MAXIMIZED,
    borderless = SDL_WINDOW_BORDERLESS,
    resizable = SDL_WINDOW_RESIZABLE,
    skip_taskbar = SDL_WINDOW_SKIP_TASKBAR,
    input_grabbed = SDL_WINDOW_INPUT_GRABBED,
    input_focus = SDL_WINDOW_INPUT_FOCUS,
    mouse_focus = SDL_WINDOW_MOUSE_FOCUS,
    mouse_capture = SDL_WINDOW_MOUSE_CAPTURE,
    allow_high_dpi = SDL_WINDOW_ALLOW_HIGHDPI,
    always_on_top = SDL_WINDOW_ALWAYS_ON_TOP,
    foreign = SDL_WINDOW_FOREIGN,
    utility = SDL_WINDOW_UTILITY,
    tooltip = SDL_WINDOW_TOOLTIP,
    popup_menu = SDL_WINDOW_POPUP_MENU,
    opengl = SDL_WINDOW_OPENGL,
    vulkan = SDL_WINDOW_VULKAN,
  };
  explicit basic_window(maybe_owner<SDL_Window*> window) noexcept(detail::is_handle<T>)
      : mWindow{window}
  {
    if constexpr (detail::is_owner<T>) {
      if (!mWindow) {
        throw exception{"Cannot create window from null pointer!"};
      }
    }
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit basic_window(const char* title,
                        const iarea size = default_size(),
                        const uint32 flags = default_flags())
  {
    assert(title);
    if (size.width < 1) {
      throw exception{"Bad window width!"};
    }
    else if (size.height < 1) {
      throw exception{"Bad window height!"};
    }
    mWindow.reset(SDL_CreateWindow(title,
                                   SDL_WINDOWPOS_CENTERED,
                                   SDL_WINDOWPOS_CENTERED,
                                   size.width,
                                   size.height,
                                   flags));
    if (!mWindow) {
      throw sdl_error{};
    }
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit basic_window(const std::string& title,
                        const iarea size = default_size(),
                        const uint32 flags = default_flags())
      : basic_window{title.c_str(), size, flags}
  {}
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  basic_window() : basic_window{"Centurion"}
  {}
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit basic_window(const window& owner) noexcept : mWindow{owner.get()}
  {}
  [[nodiscard]] auto make_renderer(const uint32 flags = renderer::default_flags()) -> renderer
  {
    if (auto* ptr = SDL_CreateRenderer(get(), -1, flags)) {
      return renderer{ptr};
    }
    else {
      throw sdl_error{};
    }
  }
  [[nodiscard]] auto get_renderer() noexcept -> renderer_handle
  {
    return renderer_handle{SDL_GetRenderer(mWindow)};
  }
  void show() noexcept { SDL_ShowWindow(mWindow); }
  void hide() noexcept { SDL_HideWindow(mWindow); }
  void raise() noexcept { SDL_RaiseWindow(mWindow); }
  void maximize() noexcept { SDL_MaximizeWindow(mWindow); }
  void minimize() noexcept { SDL_MinimizeWindow(mWindow); }
  void restore() noexcept { SDL_RestoreWindow(mWindow); }
  auto update_surface() noexcept -> result { return SDL_UpdateWindowSurface(mWindow) == 0; }
  auto set_fullscreen(const bool enabled) noexcept -> result
  {
    return SDL_SetWindowFullscreen(mWindow, enabled ? fullscreen : 0) == 0;
  }
  auto set_fullscreen_desktop(const bool enabled) noexcept -> result
  {
    return SDL_SetWindowFullscreen(mWindow, enabled ? fullscreen_desktop : 0) == 0;
  }
  void set_decorated(const bool decorated) noexcept
  {
    SDL_SetWindowBordered(mWindow, decorated ? SDL_TRUE : SDL_FALSE);
  }
  void set_resizable(const bool resizable) noexcept
  {
    SDL_SetWindowResizable(mWindow, resizable ? SDL_TRUE : SDL_FALSE);
  }
  void set_icon(const surface& icon) noexcept { SDL_SetWindowIcon(mWindow, icon.get()); }
  void set_title(const char* title) noexcept
  {
    assert(title);
    SDL_SetWindowTitle(mWindow, title);
  }
  void set_title(const std::string& title) noexcept { set_title(title.c_str()); }
  auto set_opacity(const float opacity) noexcept -> result
  {
    return SDL_SetWindowOpacity(mWindow, opacity) == 0;
  }
  auto set_brightness(const float brightness) noexcept -> result
  {
    return SDL_SetWindowBrightness(mWindow, detail::clamp(brightness, 0.0f, 1.0f)) == 0;
  }
  void center() noexcept { set_position({SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED}); }
  void set_position(const ipoint position) noexcept
  {
    SDL_SetWindowPosition(mWindow, position.x(), position.y());
  }
  void set_x(const int x) noexcept { set_position({x, y()}); }
  void set_y(const int y) noexcept { set_position({x(), y}); }
  [[nodiscard]] auto position() const noexcept -> ipoint
  {
    int x{};
    int y{};
    SDL_GetWindowPosition(mWindow, &x, &y);
    return {x, y};
  }
  [[nodiscard]] auto x() const noexcept -> int { return position().x(); }
  [[nodiscard]] auto y() const noexcept -> int { return position().y(); }
  void set_size(const iarea size) noexcept
  {
    SDL_SetWindowSize(mWindow, detail::max(size.width, 1), detail::max(size.height, 1));
  }
  void set_width(const int width) noexcept
  {
    SDL_SetWindowSize(mWindow, detail::max(width, 1), height());
  }
  void set_height(const int height) noexcept
  {
    SDL_SetWindowSize(mWindow, width(), detail::max(height, 1));
  }
  void set_min_size(const iarea size) noexcept
  {
    SDL_SetWindowMinimumSize(mWindow, detail::max(size.width, 1), detail::max(size.height, 1));
  }
  void set_max_size(const iarea size) noexcept
  {
    SDL_SetWindowMaximumSize(mWindow, detail::max(size.width, 1), detail::max(size.height, 1));
  }
  [[nodiscard]] auto size() const noexcept -> iarea
  {
    iarea size{};
    SDL_GetWindowSize(mWindow, &size.width, &size.height);
    return size;
  }
  [[nodiscard]] auto width() const noexcept -> int { return size().width; }
  [[nodiscard]] auto height() const noexcept -> int { return size().height; }
  [[nodiscard]] auto min_size() const noexcept -> iarea
  {
    iarea size{};
    SDL_GetWindowMinimumSize(mWindow, &size.width, &size.height);
    return size;
  }
  [[nodiscard]] auto max_size() const noexcept -> iarea
  {
    iarea size{};
    SDL_GetWindowMaximumSize(mWindow, &size.width, &size.height);
    return size;
  }
  static auto set_capturing_mouse(const bool capture) noexcept -> result
  {
    return SDL_CaptureMouse(capture ? SDL_TRUE : SDL_FALSE) == 0;
  }
  void set_grab_mouse(const bool grab) noexcept
  {
    SDL_SetWindowGrab(mWindow, grab ? SDL_TRUE : SDL_FALSE);
  }
  [[nodiscard]] auto is_grabbing_mouse() const noexcept -> bool
  {
    return SDL_GetWindowGrab(mWindow);
  }
  [[nodiscard]] auto title() const -> std::string { return SDL_GetWindowTitle(mWindow); }
  [[nodiscard]] auto brightness() const noexcept -> float
  {
    return SDL_GetWindowBrightness(mWindow);
  }
  [[nodiscard]] auto opacity() const noexcept -> float
  {
    float opacity{1};
    SDL_GetWindowOpacity(mWindow, &opacity);
    return opacity;
  }
  [[nodiscard]] auto id() const noexcept -> uint32 { return SDL_GetWindowID(mWindow); }
  [[nodiscard]] auto display_index() const noexcept -> maybe<int>
  {
    const auto index = SDL_GetWindowDisplayIndex(mWindow);
    if (index != -1) {
      return index;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto format() const noexcept -> pixel_format
  {
    return static_cast<pixel_format>(SDL_GetWindowPixelFormat(mWindow));
  }
  [[nodiscard]] auto is_screen_keyboard_shown() const noexcept -> bool
  {
    return SDL_IsScreenKeyboardShown(get()) == SDL_TRUE;
  }
  [[nodiscard]] auto flags() const noexcept -> uint32 { return SDL_GetWindowFlags(mWindow); }
  [[nodiscard]] auto check_flag(const uint32 flag) const noexcept -> bool
  {
    return flags() & flag;
  }
  [[nodiscard]] auto has_grabbed_input() const noexcept -> bool
  {
    return check_flag(input_grabbed);
  }
  [[nodiscard]] auto has_input_focus() const noexcept -> bool
  {
    return check_flag(input_focus);
  }
  [[nodiscard]] auto has_mouse_focus() const noexcept -> bool
  {
    return check_flag(mouse_focus);
  }
  [[nodiscard]] auto is_borderless() const noexcept -> bool { return check_flag(borderless); }
  [[nodiscard]] auto is_decorated() const noexcept -> bool { return !is_borderless(); }
  [[nodiscard]] auto is_resizable() const noexcept -> bool { return check_flag(resizable); }
  [[nodiscard]] auto is_high_dpi() const noexcept -> bool
  {
    return check_flag(allow_high_dpi);
  }
  [[nodiscard]] auto is_fullscreen() const noexcept -> bool { return check_flag(fullscreen); }
  [[nodiscard]] auto is_fullscreen_desktop() const noexcept -> bool
  {
    return check_flag(fullscreen_desktop);
  }
  [[nodiscard]] auto is_visible() const noexcept -> bool { return check_flag(shown); }
  [[nodiscard]] auto is_hidden() const noexcept -> bool { return check_flag(hidden); }
  [[nodiscard]] auto is_opengl() const noexcept -> bool { return check_flag(opengl); }
  [[nodiscard]] auto is_vulkan() const noexcept -> bool { return check_flag(vulkan); }
  [[nodiscard]] auto is_foreign() const noexcept -> bool { return check_flag(foreign); }
  [[nodiscard]] auto is_capturing_mouse() const noexcept -> bool
  {
    return check_flag(mouse_capture);
  }
  [[nodiscard]] auto is_minimized() const noexcept -> bool { return check_flag(minimized); }
  [[nodiscard]] auto is_maximized() const noexcept -> bool { return check_flag(maximized); }
  [[nodiscard]] auto is_always_on_top() const noexcept -> bool
  {
    return check_flag(always_on_top);
  }
  [[nodiscard]] auto is_utility() const noexcept -> bool { return check_flag(utility); }
  [[nodiscard]] auto is_tooltip() const noexcept -> bool { return check_flag(tooltip); }
  [[nodiscard]] auto is_popup_menu() const noexcept -> bool { return check_flag(popup_menu); }
  [[nodiscard]] auto is_excluded_from_taskbar() const noexcept -> bool
  {
    return check_flag(skip_taskbar);
  }
  [[nodiscard]] auto get_surface() noexcept -> surface_handle
  {
    return surface_handle{SDL_GetWindowSurface(mWindow)};
  }
  [[nodiscard]] auto get() const noexcept -> SDL_Window* { return mWindow.get(); }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit operator bool() const noexcept
  {
    return mWindow != nullptr;
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] constexpr static auto default_size() noexcept -> iarea
  {
    return {800, 600};
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  [[nodiscard]] constexpr static auto default_flags() noexcept -> uint32
  {
    return hidden;
  }
 private:
  detail::pointer<T, SDL_Window> mWindow;
};
template <typename T>
[[nodiscard]] auto to_string(const basic_window<T>& window) -> std::string
{
  return "window(data: " + detail::address_of(window.get()) +
         ", width: " + std::to_string(window.width()) +
         ", height: " + std::to_string(window.height()) + ")";
}
template <typename T>
auto operator<<(std::ostream& stream, const basic_window<T>& window) -> std::ostream&
{
  return stream << to_string(window);
}
[[nodiscard]] inline auto get_grabbed_window() noexcept -> window_handle
{
  return window_handle{SDL_GetGrabbedWindow()};
}
[[nodiscard]] inline auto get_mouse_focus_window() noexcept -> window_handle
{
  return window_handle{SDL_GetMouseFocus()};
}
[[nodiscard]] inline auto get_keyboard_focus_window() noexcept -> window_handle
{
  return window_handle{SDL_GetKeyboardFocus()};
}
[[nodiscard]] inline auto get_window(const uint32 id) noexcept -> window_handle
{
  return window_handle{SDL_GetWindowFromID(id)};
}
}
namespace cen {
enum class system_cursor
{
  arrow = SDL_SYSTEM_CURSOR_ARROW,
  ibeam = SDL_SYSTEM_CURSOR_IBEAM,
  wait = SDL_SYSTEM_CURSOR_WAIT,
  crosshair = SDL_SYSTEM_CURSOR_CROSSHAIR,
  wait_arrow = SDL_SYSTEM_CURSOR_WAITARROW,
  size_nw_se = SDL_SYSTEM_CURSOR_SIZENWSE,
  size_ne_sw = SDL_SYSTEM_CURSOR_SIZENESW,
  size_we = SDL_SYSTEM_CURSOR_SIZEWE,
  size_ns = SDL_SYSTEM_CURSOR_SIZENS,
  size_all = SDL_SYSTEM_CURSOR_SIZEALL,
  no = SDL_SYSTEM_CURSOR_NO,
  hand = SDL_SYSTEM_CURSOR_HAND
};
[[nodiscard]] constexpr auto system_cursor_count() noexcept -> int
{
  return SDL_NUM_SYSTEM_CURSORS;
}
[[nodiscard]] constexpr auto to_string(const system_cursor cursor) -> std::string_view
{
  switch (cursor) {
    case system_cursor::arrow:
      return "arrow";
    case system_cursor::ibeam:
      return "ibeam";
    case system_cursor::wait:
      return "wait";
    case system_cursor::crosshair:
      return "crosshair";
    case system_cursor::wait_arrow:
      return "wait_arrow";
    case system_cursor::size_nw_se:
      return "size_nw_se";
    case system_cursor::size_ne_sw:
      return "size_ne_sw";
    case system_cursor::size_we:
      return "size_we";
    case system_cursor::size_ns:
      return "size_ns";
    case system_cursor::size_all:
      return "size_all";
    case system_cursor::no:
      return "no";
    case system_cursor::hand:
      return "hand";
    default:
      throw exception{"Did not recognize system cursor!"};
  }
}
inline auto operator<<(std::ostream& stream, const system_cursor cursor) -> std::ostream&
{
  return stream << to_string(cursor);
}
enum class mouse_button : uint8
{
  left = SDL_BUTTON_LEFT,
  middle = SDL_BUTTON_MIDDLE,
  right = SDL_BUTTON_RIGHT,
  x1 = SDL_BUTTON_X1,
  x2 = SDL_BUTTON_X2
};
[[nodiscard]] constexpr auto to_string(const mouse_button button) -> std::string_view
{
  switch (button) {
    case mouse_button::left:
      return "left";
    case mouse_button::middle:
      return "middle";
    case mouse_button::right:
      return "right";
    case mouse_button::x1:
      return "x1";
    case mouse_button::x2:
      return "x2";
    default:
      throw exception{"Did not recognize mouse button!"};
  }
}
inline auto operator<<(std::ostream& stream, const mouse_button button) -> std::ostream&
{
  return stream << to_string(button);
}
class mouse final
{
 public:
  mouse() noexcept = default;
  static auto warp(const ipoint& pos) noexcept -> result
  {
    return SDL_WarpMouseGlobal(pos.x(), pos.y()) == 0;
  }
  static void warp_in_window(const ipoint& pos) noexcept
  {
    SDL_WarpMouseInWindow(nullptr, pos.x(), pos.y());
  }
  template <typename T>
  static void warp_in_window(const basic_window<T>& window, const ipoint& pos) noexcept
  {
    SDL_WarpMouseInWindow(window.get(), pos.x(), pos.y());
  }
  static auto set_relative_mode(const bool enabled) noexcept -> result
  {
    return SDL_SetRelativeMouseMode(enabled ? SDL_TRUE : SDL_FALSE) == 0;
  }
  [[nodiscard]] static auto is_relative_mode_enabled() noexcept -> bool
  {
    return SDL_GetRelativeMouseMode() == SDL_TRUE;
  }
  void update() noexcept
  {
    mPreviousMask = mCurrentMask;
    mPreviousPosition = mCurrentPosition;
    int mx{};
    int my{};
    mCurrentMask = SDL_GetMouseState(&mx, &my);
    mCurrentPosition.set_x(mx);
    mCurrentPosition.set_y(my);
  }
  [[nodiscard]] auto position() const noexcept -> ipoint { return mCurrentPosition; }
  [[nodiscard]] auto x() const noexcept -> int { return mCurrentPosition.x(); }
  [[nodiscard]] auto y() const noexcept -> int { return mCurrentPosition.y(); }
  [[nodiscard]] auto is_left_pressed() const noexcept -> bool
  {
    return is_pressed(SDL_BUTTON_LMASK);
  }
  [[nodiscard]] auto is_middle_pressed() const noexcept -> bool
  {
    return is_pressed(SDL_BUTTON_MMASK);
  }
  [[nodiscard]] auto is_right_pressed() const noexcept -> bool
  {
    return is_pressed(SDL_BUTTON_RMASK);
  }
  [[nodiscard]] auto was_left_released() const noexcept -> bool
  {
    return was_released(SDL_BUTTON_LMASK);
  }
  [[nodiscard]] auto was_middle_released() const noexcept -> bool
  {
    return was_released(SDL_BUTTON_MMASK);
  }
  [[nodiscard]] auto was_right_released() const noexcept -> bool
  {
    return was_released(SDL_BUTTON_RMASK);
  }
  [[nodiscard]] auto was_moved() const noexcept -> bool
  {
    return (mCurrentPosition.x() != mPreviousPosition.x()) ||
           (mCurrentPosition.y() != mPreviousPosition.y());
  }
  [[nodiscard]] static auto position_relative_window() noexcept -> ipoint
  {
    int x{};
    int y{};
    SDL_GetMouseState(&x, &y);
    return {x, y};
  }
  [[nodiscard]] static auto position_relative_desktop() noexcept -> ipoint
  {
    int x{};
    int y{};
    SDL_GetGlobalMouseState(&x, &y);
    return {x, y};
  }
  [[nodiscard]] static auto delta() noexcept -> ipoint
  {
    int x{};
    int y{};
    SDL_GetRelativeMouseState(&x, &y);
    return {x, y};
  }
  [[nodiscard]] static auto is_haptic() noexcept -> bool
  {
    return SDL_MouseIsHaptic() == SDL_TRUE;
  }
 private:
  ipoint mCurrentPosition;
  ipoint mPreviousPosition;
  uint32 mCurrentMask{};
  uint32 mPreviousMask{};
  [[nodiscard]] auto is_pressed(const uint32 mask) const noexcept -> bool
  {
    return mCurrentMask & mask;
  }
  [[nodiscard]] auto was_released(const uint32 mask) const noexcept -> bool
  {
    return !(mCurrentMask & mask) && mPreviousMask & mask;
  }
};
[[nodiscard]] inline auto to_string(const mouse& mouse) -> std::string
{
  return "mouse(x: " + std::to_string(mouse.x()) + ", y: " + std::to_string(mouse.y()) + ")";
}
inline auto operator<<(std::ostream& stream, const mouse& mouse) -> std::ostream&
{
  return stream << to_string(mouse);
}
template <typename T>
class basic_cursor;
using cursor = basic_cursor<detail::owner_tag>;
using cursor_handle = basic_cursor<detail::handle_tag>;
template <typename T>
class basic_cursor final
{
 public:
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  explicit basic_cursor(const system_cursor cursor)
      : mCursor{SDL_CreateSystemCursor(static_cast<SDL_SystemCursor>(cursor))}
  {
    if (!mCursor) {
      throw sdl_error{};
    }
  }
  template <typename TT = T, detail::enable_for_owner<TT> = 0>
  basic_cursor(const surface& surface, const ipoint& hotspot)
      : mCursor{SDL_CreateColorCursor(surface.get(), hotspot.x(), hotspot.y())}
  {
    if (!mCursor) {
      throw sdl_error{};
    }
  }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit basic_cursor(SDL_Cursor* cursor) noexcept : mCursor{cursor}
  {}
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit basic_cursor(const cursor& owner) noexcept : mCursor{owner.get()}
  {}
  void enable() noexcept { SDL_SetCursor(mCursor); }
  [[nodiscard]] auto is_enabled() const noexcept -> bool { return SDL_GetCursor() == get(); }
  static void reset() noexcept { SDL_SetCursor(SDL_GetDefaultCursor()); }
  static void force_redraw() noexcept { SDL_SetCursor(nullptr); }
  static void set_visible(const bool visible) noexcept
  {
    SDL_ShowCursor(visible ? SDL_ENABLE : SDL_DISABLE);
  }
  [[nodiscard]] static auto visible() noexcept -> bool
  {
    return SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE;
  }
  [[nodiscard]] static auto get_default() noexcept -> cursor_handle
  {
    return cursor_handle{SDL_GetDefaultCursor()};
  }
  [[nodiscard]] static auto get_current() noexcept -> cursor_handle
  {
    return cursor_handle{SDL_GetCursor()};
  }
  [[nodiscard]] auto get() const noexcept -> SDL_Cursor* { return mCursor.get(); }
  template <typename TT = T, detail::enable_for_handle<TT> = 0>
  explicit operator bool() const noexcept
  {
    return mCursor != nullptr;
  }
 private:
  detail::pointer<T, SDL_Cursor> mCursor;
};
}
namespace cen {
class quit_event final : public event_base<SDL_QuitEvent>
{
 public:
  quit_event() : event_base{event_type::quit} {}
  explicit quit_event(const SDL_QuitEvent& event) noexcept : event_base{event} {}
};
template <>
inline auto as_sdl_event(const event_base<SDL_QuitEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.quit = event.get();
  return e;
}
class dollar_gesture_event final : public event_base<SDL_DollarGestureEvent>
{
 public:
  dollar_gesture_event() : event_base{event_type::dollar_gesture} {}
  explicit dollar_gesture_event(const SDL_DollarGestureEvent& event) noexcept
      : event_base{event}
  {}
  void set_touch_id(const SDL_TouchID id) noexcept { mEvent.touchId = id; }
  void set_gesture_id(const SDL_GestureID id) noexcept { mEvent.gestureId = id; }
  void set_fingers(const uint32 fingers) noexcept { mEvent.numFingers = fingers; }
  void set_error(const float error) noexcept { mEvent.error = error; }
  void set_x(const float x) noexcept { mEvent.x = x; }
  void set_y(const float y) noexcept { mEvent.y = y; }
  [[nodiscard]] auto touch_id() const noexcept -> SDL_TouchID { return mEvent.touchId; }
  [[nodiscard]] auto gesture_id() const noexcept -> SDL_GestureID { return mEvent.gestureId; }
  [[nodiscard]] auto finger_count() const noexcept -> uint32 { return mEvent.numFingers; }
  [[nodiscard]] auto error() const noexcept -> float { return mEvent.error; }
  [[nodiscard]] auto x() const noexcept -> float { return mEvent.x; }
  [[nodiscard]] auto y() const noexcept -> float { return mEvent.y; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_DollarGestureEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.dgesture = event.get();
  return e;
}
class drop_event final : public event_base<SDL_DropEvent>
{
 public:
  drop_event() : event_base{event_type::drop_file} {}
  explicit drop_event(const SDL_DropEvent& event) noexcept : event_base{event} {}
  ~drop_event() noexcept { maybe_destroy_file(); }
  void set_will_free_file(const bool freeFile) noexcept { mFreeFile = freeFile; }
  void set_file(char* file) noexcept
  {
    maybe_destroy_file();
    mEvent.file = file;
  }
  void set_window_id(const uint32 id) noexcept { mEvent.windowID = id; }
  [[nodiscard]] auto will_free_file() const noexcept -> bool { return mFreeFile; }
  [[nodiscard]] auto file() const noexcept -> char* { return mEvent.file; }
  [[nodiscard]] auto window_id() const noexcept -> uint32 { return mEvent.windowID; }
 private:
  bool mFreeFile{false};
  void maybe_destroy_file() noexcept
  {
    if (mEvent.file && mFreeFile) {
      SDL_free(mEvent.file);
    }
  }
};
template <>
inline auto as_sdl_event(const event_base<SDL_DropEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.drop = event.get();
  return e;
}
class keyboard_event final : public event_base<SDL_KeyboardEvent>
{
 public:
  keyboard_event() : event_base{event_type::key_down} {}
  explicit keyboard_event(const SDL_KeyboardEvent& event) noexcept : event_base{event} {}
  void set_scan(const scan_code code) noexcept { mEvent.keysym.scancode = code.get(); }
  void set_key(const key_code code) noexcept { mEvent.keysym.sym = code.get(); }
  void set_state(const button_state state) noexcept { mEvent.state = to_underlying(state); }
  void set_modifier(const key_mod mod, const bool active) noexcept
  {
    if (active) {
      mEvent.keysym.mod |= to_underlying(mod);
    }
    else {
      mEvent.keysym.mod &= ~to_underlying(mod);
    }
  }
  void set_repeated(const bool repeated) noexcept { mEvent.repeat = repeated; }
  void set_window_id(const uint32 id) noexcept { mEvent.windowID = id; }
  [[nodiscard]] auto is_active(const scan_code& code) const noexcept -> bool
  {
    return mEvent.keysym.scancode == code.get();
  }
  [[nodiscard]] auto is_active(const key_code& code) const noexcept -> bool
  {
    return static_cast<SDL_KeyCode>(mEvent.keysym.sym) == code.get();
  }
  [[nodiscard]] auto is_active(const key_mod mod) const noexcept -> bool
  {
    return detail::is_active(mod, mEvent.keysym.mod);
  }
  [[nodiscard]] auto is_only_active(const key_mod mod) const noexcept -> bool
  {
    return detail::is_only_active(mod, mEvent.keysym.mod);
  }
  [[nodiscard]] auto is_only_subset_active(const key_mod mod) const noexcept -> bool
  {
    return detail::is_only_subset_active(mod, mEvent.keysym.mod);
  }
  [[nodiscard]] auto repeated() const noexcept -> bool { return mEvent.repeat; }
  [[nodiscard]] auto state() const noexcept -> button_state
  {
    return static_cast<button_state>(mEvent.state);
  }
  [[nodiscard]] auto released() const noexcept -> bool
  {
    return state() == button_state::released;
  }
  [[nodiscard]] auto pressed() const noexcept -> bool
  {
    return state() == button_state::pressed;
  }
  [[nodiscard]] auto scan() const noexcept -> scan_code { return mEvent.keysym.scancode; }
  [[nodiscard]] auto key() const noexcept -> key_code
  {
    return static_cast<SDL_KeyCode>(mEvent.keysym.sym);
  }
  [[nodiscard]] auto window_id() const noexcept -> uint32 { return mEvent.windowID; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_KeyboardEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.key = event.get();
  return e;
}
class multi_gesture_event final : public event_base<SDL_MultiGestureEvent>
{
 public:
  multi_gesture_event() : event_base{event_type::multi_gesture} {}
  explicit multi_gesture_event(const SDL_MultiGestureEvent& event) noexcept : event_base{event}
  {}
  void set_touch_id(const SDL_TouchID id) noexcept { mEvent.touchId = id; }
  void set_delta_theta(const float dTheta) noexcept { mEvent.dTheta = dTheta; }
  void set_delta_distance(const float dDistance) noexcept { mEvent.dDist = dDistance; }
  void set_center_x(const float centerX) noexcept { mEvent.x = centerX; }
  void set_center_y(const float centerY) noexcept { mEvent.y = centerY; }
  void set_finger_count(const uint16 count) noexcept { mEvent.numFingers = count; }
  [[nodiscard]] auto touch_id() const noexcept -> SDL_TouchID { return mEvent.touchId; }
  [[nodiscard]] auto delta_theta() const noexcept -> float { return mEvent.dTheta; }
  [[nodiscard]] auto delta_distance() const noexcept -> float { return mEvent.dDist; }
  [[nodiscard]] auto center_x() const noexcept -> float { return mEvent.x; }
  [[nodiscard]] auto center_y() const noexcept -> float { return mEvent.y; }
  [[nodiscard]] auto finger_count() const noexcept -> uint16 { return mEvent.numFingers; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_MultiGestureEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.mgesture = event.get();
  return e;
}
class sensor_event final : public event_base<SDL_SensorEvent>
{
 public:
  using data_type = std::array<float, 6>;
  sensor_event() : event_base{event_type::sensor_update} {}
  explicit sensor_event(const SDL_SensorEvent& event) noexcept : event_base{event} {}
  void set_which(const int32 id) noexcept { mEvent.which = id; }
  void set_data(const data_type& values) { detail::assign(values, mEvent.data); }
  [[nodiscard]] auto which() const noexcept -> int32 { return mEvent.which; }
  [[nodiscard]] auto data() const -> data_type { return detail::to_array(mEvent.data); }
};
template <>
inline auto as_sdl_event(const event_base<SDL_SensorEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.sensor = event.get();
  return e;
}
class text_editing_event final : public event_base<SDL_TextEditingEvent>
{
 public:
  text_editing_event() : event_base{event_type::text_editing} { check_length(); }
  explicit text_editing_event(const SDL_TextEditingEvent& event) noexcept : event_base{event}
  {
    check_length();
  }
  void set_window_id(const uint32 id) noexcept { mEvent.windowID = id; }
  void set_start(const int32 start) noexcept { mEvent.start = start; }
  void set_length(const int32 length) noexcept
  {
    mEvent.length = detail::clamp(length, 0, SDL_TEXTEDITINGEVENT_TEXT_SIZE);
  }
  [[nodiscard]] auto window_id() const noexcept -> uint32 { return mEvent.windowID; }
  [[nodiscard]] auto text() const noexcept -> std::string_view
  {
    return std::string_view{static_cast<const char*>(mEvent.text)};
  }
  [[nodiscard]] auto start() const noexcept -> int32 { return mEvent.start; }
  [[nodiscard]] auto length() const noexcept -> int32 { return mEvent.length; }
 private:
  void check_length() noexcept
  {
    mEvent.length = detail::clamp(mEvent.length, 0, SDL_TEXTEDITINGEVENT_TEXT_SIZE);
  }
};
template <>
inline auto as_sdl_event(const event_base<SDL_TextEditingEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.edit = event.get();
  return e;
}
class text_input_event final : public event_base<SDL_TextInputEvent>
{
 public:
  text_input_event() : event_base{event_type::text_input} {}
  explicit text_input_event(const SDL_TextInputEvent& event) noexcept : event_base{event} {}
  void set_window_id(const uint32 id) noexcept { mEvent.windowID = id; }
  [[nodiscard]] auto window_id() const noexcept -> uint32 { return mEvent.windowID; }
  [[nodiscard]] auto text_utf8() const noexcept -> std::string_view
  {
    return std::string_view{static_cast<const char*>(mEvent.text)};
  }
};
template <>
inline auto as_sdl_event(const event_base<SDL_TextInputEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.text = event.get();
  return e;
}
class touch_finger_event final : public event_base<SDL_TouchFingerEvent>
{
 public:
  touch_finger_event() : event_base{event_type::finger_down} {}
  explicit touch_finger_event(const SDL_TouchFingerEvent& event) noexcept : event_base{event}
  {}
  void set_touch_id(const SDL_TouchID id) noexcept { mEvent.touchId = id; }
  void set_finger_id(const SDL_FingerID id) noexcept { mEvent.fingerId = id; }
  void set_x(const float x) noexcept { mEvent.x = detail::clamp(x, 0.0f, 1.0f); }
  void set_y(const float y) noexcept { mEvent.y = detail::clamp(y, 0.0f, 1.0f); }
  void set_dx(const float dx) noexcept { mEvent.dx = detail::clamp(dx, -1.0f, 1.0f); }
  void set_dy(const float dy) noexcept { mEvent.dy = detail::clamp(dy, -1.0f, 1.0f); }
  void set_pressure(const float pressure) noexcept
  {
    mEvent.pressure = detail::clamp(pressure, 0.0f, 1.0f);
  }
  [[nodiscard]] auto touch_id() const noexcept -> SDL_TouchID { return mEvent.touchId; }
  [[nodiscard]] auto finger_id() const noexcept -> SDL_FingerID { return mEvent.fingerId; }
  [[nodiscard]] auto x() const noexcept -> float { return mEvent.x; }
  [[nodiscard]] auto y() const noexcept -> float { return mEvent.y; }
  [[nodiscard]] auto dx() const noexcept -> float { return mEvent.dx; }
  [[nodiscard]] auto dy() const noexcept -> float { return mEvent.dy; }
  [[nodiscard]] auto pressure() const noexcept -> float { return mEvent.pressure; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_TouchFingerEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.tfinger = event.get();
  return e;
}
class user_event final : public event_base<SDL_UserEvent>
{
 public:
  user_event() : event_base{event_type::user} {}
  explicit user_event(const SDL_UserEvent& event) noexcept : event_base{event} {}
  void set_window_id(const uint32 id) noexcept { mEvent.windowID = id; }
  void set_code(const int32 code) noexcept { mEvent.code = code; }
  void set_data1(void* data) noexcept { mEvent.data1 = data; }
  void set_data2(void* data) noexcept { mEvent.data2 = data; }
  [[nodiscard]] auto window_id() const noexcept -> uint32 { return mEvent.windowID; }
  [[nodiscard]] auto code() const noexcept -> int32 { return mEvent.code; }
  [[nodiscard]] auto data1() noexcept -> void* { return mEvent.data1; }
  [[nodiscard]] auto data1() const noexcept -> const void* { return mEvent.data1; }
  [[nodiscard]] auto data2() noexcept -> void* { return mEvent.data2; }
  [[nodiscard]] auto data2() const noexcept -> const void* { return mEvent.data2; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_UserEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.user = event.get();
  return e;
}
}
namespace cen {
enum class mouse_wheel_direction : uint32
{
  normal = SDL_MOUSEWHEEL_NORMAL,
  flipped = SDL_MOUSEWHEEL_FLIPPED
};
[[nodiscard]] constexpr auto to_string(const mouse_wheel_direction dir) -> std::string_view
{
  switch (dir) {
    case mouse_wheel_direction::normal:
      return "normal";
    case mouse_wheel_direction::flipped:
      return "flipped";
    default:
      throw exception{"Did not recognize mouse wheel direction!"};
  }
}
inline auto operator<<(std::ostream& stream, const mouse_wheel_direction dir) -> std::ostream&
{
  return stream << to_string(dir);
}
class mouse_button_event final : public event_base<SDL_MouseButtonEvent>
{
 public:
  mouse_button_event() : event_base{event_type::mouse_button_down} {}
  explicit mouse_button_event(const SDL_MouseButtonEvent& event) noexcept : event_base{event}
  {}
  void set_window_id(const uint32 id) noexcept { mEvent.windowID = id; }
  void set_which(const uint32 which) noexcept { mEvent.which = which; }
  void set_button(const mouse_button button) noexcept
  {
    mEvent.button = to_underlying(button);
  }
  void set_state(const button_state state) noexcept { mEvent.state = to_underlying(state); }
  void set_clicks(const uint8 clicks) noexcept { mEvent.clicks = clicks; }
  void set_x(const int32 x) noexcept { mEvent.x = x; }
  void set_y(const int32 y) noexcept { mEvent.y = y; }
  [[nodiscard]] auto window_id() const noexcept -> uint32 { return mEvent.windowID; }
  [[nodiscard]] auto which() const noexcept -> uint32 { return mEvent.which; }
  [[nodiscard]] auto button() const noexcept -> mouse_button
  {
    return static_cast<mouse_button>(mEvent.button);
  }
  [[nodiscard]] auto state() const noexcept -> button_state
  {
    return static_cast<button_state>(mEvent.state);
  }
  [[nodiscard]] auto pressed() const noexcept -> bool
  {
    return state() == button_state::pressed;
  }
  [[nodiscard]] auto released() const noexcept -> bool
  {
    return state() == button_state::released;
  }
  [[nodiscard]] auto clicks() const noexcept -> uint8 { return mEvent.clicks; }
  [[nodiscard]] auto x() const noexcept -> int32 { return mEvent.x; }
  [[nodiscard]] auto y() const noexcept -> int32 { return mEvent.y; }
  [[nodiscard]] auto position() const noexcept -> ipoint { return {x(), y()}; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_MouseButtonEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.button = event.get();
  return e;
}
class mouse_motion_event final : public event_base<SDL_MouseMotionEvent>
{
 public:
  mouse_motion_event() : event_base{event_type::mouse_motion} {}
  explicit mouse_motion_event(const SDL_MouseMotionEvent& event) noexcept : event_base{event}
  {}
  void set_window_id(const uint32 id) noexcept { mEvent.windowID = id; }
  void set_which(const uint32 which) noexcept { mEvent.which = which; }
  void set_state(const uint32 state) noexcept { mEvent.state = state; }
  void set_x(const int32 x) noexcept { mEvent.x = x; }
  void set_y(const int32 y) noexcept { mEvent.y = y; }
  void set_dx(const int32 dx) noexcept { mEvent.xrel = dx; }
  void set_dy(const int32 dy) noexcept { mEvent.yrel = dy; }
  [[nodiscard]] auto window_id() const noexcept -> uint32 { return mEvent.windowID; }
  [[nodiscard]] auto which() const noexcept -> uint32 { return mEvent.which; }
  [[nodiscard]] auto state() const noexcept -> uint32 { return mEvent.state; }
  [[nodiscard]] auto pressed(const mouse_button button) const noexcept -> bool
  {
    return mEvent.state & SDL_BUTTON(to_underlying(button));
  }
  [[nodiscard]] auto x() const noexcept -> int32 { return mEvent.x; }
  [[nodiscard]] auto y() const noexcept -> int32 { return mEvent.y; }
  [[nodiscard]] auto dx() const noexcept -> int32 { return mEvent.xrel; }
  [[nodiscard]] auto dy() const noexcept -> int32 { return mEvent.yrel; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_MouseMotionEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.motion = event.get();
  return e;
}
class mouse_wheel_event final : public event_base<SDL_MouseWheelEvent>
{
 public:
  mouse_wheel_event() : event_base{event_type::mouse_wheel} {}
  explicit mouse_wheel_event(const SDL_MouseWheelEvent& event) noexcept : event_base{event} {}
  void set_window_id(const uint32 id) noexcept { mEvent.windowID = id; }
  void set_which(const uint32 which) noexcept { mEvent.which = which; }
  void set_x(const int32 xScroll) noexcept { mEvent.x = xScroll; }
  void set_y(const int32 yScroll) noexcept { mEvent.y = yScroll; }
  void set_direction(const mouse_wheel_direction direction) noexcept
  {
    mEvent.direction = to_underlying(direction);
  }
  [[nodiscard]] auto window_id() const noexcept -> uint32 { return mEvent.windowID; }
  [[nodiscard]] auto which() const noexcept -> uint32 { return mEvent.which; }
  [[nodiscard]] auto x() const noexcept -> int32 { return mEvent.x; }
  [[nodiscard]] auto y() const noexcept -> int32 { return mEvent.y; }
  [[nodiscard]] auto direction() const noexcept -> mouse_wheel_direction
  {
    return static_cast<mouse_wheel_direction>(mEvent.direction);
  }
};
template <>
inline auto as_sdl_event(const event_base<SDL_MouseWheelEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.wheel = event.get();
  return e;
}
}
namespace cen {
enum class window_event_id
{
  none = SDL_WINDOWEVENT_NONE,
  shown = SDL_WINDOWEVENT_SHOWN,
  hidden = SDL_WINDOWEVENT_HIDDEN,
  exposed = SDL_WINDOWEVENT_EXPOSED,
  moved = SDL_WINDOWEVENT_MOVED,
  resized = SDL_WINDOWEVENT_RESIZED,
  size_changed = SDL_WINDOWEVENT_SIZE_CHANGED,
  minimized = SDL_WINDOWEVENT_MINIMIZED,
  maximized = SDL_WINDOWEVENT_MAXIMIZED,
  restored = SDL_WINDOWEVENT_RESTORED,
  enter = SDL_WINDOWEVENT_ENTER,
  leave = SDL_WINDOWEVENT_LEAVE,
  focus_gained = SDL_WINDOWEVENT_FOCUS_GAINED,
  focus_lost = SDL_WINDOWEVENT_FOCUS_LOST,
  close = SDL_WINDOWEVENT_CLOSE,
  take_focus = SDL_WINDOWEVENT_TAKE_FOCUS,
  hit_test = SDL_WINDOWEVENT_HIT_TEST
};
[[nodiscard]] constexpr auto to_string(const window_event_id id) -> std::string_view
{
  switch (id) {
    case window_event_id::none:
      return "none";
    case window_event_id::shown:
      return "shown";
    case window_event_id::hidden:
      return "hidden";
    case window_event_id::exposed:
      return "exposed";
    case window_event_id::moved:
      return "moved";
    case window_event_id::resized:
      return "resized";
    case window_event_id::size_changed:
      return "size_changed";
    case window_event_id::minimized:
      return "minimized";
    case window_event_id::maximized:
      return "maximized";
    case window_event_id::restored:
      return "restored";
    case window_event_id::enter:
      return "enter";
    case window_event_id::leave:
      return "leave";
    case window_event_id::focus_gained:
      return "focus_gained";
    case window_event_id::focus_lost:
      return "focus_lost";
    case window_event_id::close:
      return "close";
    case window_event_id::take_focus:
      return "take_focus";
    case window_event_id::hit_test:
      return "hit_test";
    default:
      throw exception{"Did not recognize window event ID!"};
  }
}
inline auto operator<<(std::ostream& stream, const window_event_id id) -> std::ostream&
{
  return stream << to_string(id);
}
class window_event final : public event_base<SDL_WindowEvent>
{
 public:
  window_event() : event_base{event_type::window} {}
  explicit window_event(const SDL_WindowEvent& event) noexcept : event_base{event} {}
  void set_event_id(const window_event_id id) noexcept
  {
    mEvent.event = static_cast<uint8>(id);
  }
  void set_data1(const int32 value) noexcept { mEvent.data1 = value; }
  void set_data2(const int32 value) noexcept { mEvent.data2 = value; }
  [[nodiscard]] auto event_id() const noexcept -> window_event_id
  {
    return static_cast<window_event_id>(mEvent.event);
  }
  [[nodiscard]] auto data1() const noexcept -> int32 { return mEvent.data1; }
  [[nodiscard]] auto data2() const noexcept -> int32 { return mEvent.data2; }
};
template <>
inline auto as_sdl_event(const event_base<SDL_WindowEvent>& event) -> SDL_Event
{
  SDL_Event e;
  e.window = event.get();
  return e;
}
}
namespace cen {
class event_handler final
{
 public:
  static void update() noexcept { SDL_PumpEvents(); }
  template <typename T>
  static auto push(const event_base<T>& event) noexcept -> result
  {
    auto underlying = as_sdl_event(event);
    return SDL_PushEvent(&underlying) >= 0;
  }
  static void flush() noexcept { SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT); }
  static void flush_all() noexcept
  {
    SDL_PumpEvents();
    SDL_FlushEvents(SDL_FIRSTEVENT, SDL_LASTEVENT);
  }
  auto poll() noexcept -> bool
  {
    SDL_Event event{};
    if (SDL_PollEvent(&event)) {
      store(event);
      return true;
    }
    else {
      reset_state();
      return false;
    }
  }
  template <typename T>
  [[nodiscard]] auto is() const noexcept -> bool
  {
    return std::holds_alternative<T>(mData);
  }
  [[nodiscard]] auto is(const event_type type) const noexcept -> bool
  {
    if (type == event_type::user && is_user_event(mType)) {
      return true;
    }
    else {
      return mType == type;
    }
  }
  [[nodiscard]] auto type() const noexcept -> maybe<event_type>
  {
    if (mType != event_type::last_event) {
      return mType;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto raw_type() const noexcept -> maybe<uint32>
  {
    if (mType != event_type::last_event) {
      return to_underlying(mType);
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto empty() const noexcept -> bool { return is<std::monostate>(); }
  template <typename T>
  [[nodiscard]] auto get() -> T&
  {
    return std::get<T>(mData);
  }
  template <typename T>
  [[nodiscard]] auto get() const -> const T&
  {
    return std::get<T>(mData);
  }
  template <typename T>
  [[nodiscard]] auto try_get() noexcept -> T*
  {
    return std::get_if<T>(&mData);
  }
  template <typename T>
  [[nodiscard]] auto try_get() const noexcept -> const T*
  {
    return std::get_if<T>(&mData);
  }
  [[nodiscard]] static auto queue_count() noexcept -> maybe<int>
  {
    const auto num = SDL_PeepEvents(nullptr, 0, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
    if (num != -1) {
      return num;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] static auto queue_count(const event_type type) noexcept -> maybe<int>
  {
    const auto id = to_underlying(type);
    const auto num = SDL_PeepEvents(nullptr, 0, SDL_PEEKEVENT, id, id);
    if (num != -1) {
      return num;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] static auto in_queue(const event_type type) noexcept -> bool
  {
    return queue_count(type) > 0;
  }
  [[nodiscard]] auto data() const noexcept -> const SDL_Event* { return &mEvent; }
 private:
  using data_type = std::variant<std::monostate,
                                 audio_device_event,
                                 controller_axis_event,
                                 controller_button_event,
                                 controller_device_event,
                                 dollar_gesture_event,
                                 drop_event,
                                 joy_axis_event,
                                 joy_ball_event,
                                 joy_button_event,
                                 joy_device_event,
                                 joy_hat_event,
                                 keyboard_event,
                                 mouse_button_event,
                                 mouse_motion_event,
                                 mouse_wheel_event,
                                 multi_gesture_event,
                                 quit_event,
                                 text_editing_event,
                                 text_input_event,
                                 touch_finger_event,
                                 sensor_event,
                                 user_event,
                                 window_event>;
  SDL_Event mEvent{};
  event_type mType{event_type::last_event};
  data_type mData{};
  void reset_state()
  {
    mEvent = {};
    mType = event_type::last_event;
    mData.emplace<std::monostate>();
  }
  void store(const SDL_Event& event) noexcept
  {
    mEvent = event;
    const auto type = static_cast<SDL_EventType>(event.type);
    mType = static_cast<event_type>(type);
    if (is_user_event(mType)) {
      mData.emplace<user_event>(event.user);
      return;
    }
    switch (type) {
      case SDL_FIRSTEVENT:
      case SDL_LASTEVENT:
        reset_state();
        break;
      case SDL_QUIT:
        mData.emplace<quit_event>(event.quit);
        break;
      case SDL_APP_TERMINATING:
      case SDL_APP_LOWMEMORY:
      case SDL_APP_WILLENTERBACKGROUND:
      case SDL_APP_DIDENTERBACKGROUND:
      case SDL_APP_WILLENTERFOREGROUND:
      case SDL_APP_DIDENTERFOREGROUND:
        break;
      case SDL_WINDOWEVENT:
        mData.emplace<window_event>(event.window);
        break;
      case SDL_SYSWMEVENT:
        break;
      case SDL_KEYDOWN:
      case SDL_KEYUP:
        mData.emplace<keyboard_event>(event.key);
        break;
      case SDL_TEXTEDITING:
        mData.emplace<text_editing_event>(event.edit);
        break;
      case SDL_TEXTINPUT:
        mData.emplace<text_input_event>(event.text);
        break;
      case SDL_KEYMAPCHANGED:
        break;
      case SDL_MOUSEMOTION:
        mData.emplace<mouse_motion_event>(event.motion);
        break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
        mData.emplace<mouse_button_event>(event.button);
        break;
      case SDL_MOUSEWHEEL:
        mData.emplace<mouse_wheel_event>(event.wheel);
        break;
      case SDL_JOYAXISMOTION:
        mData.emplace<joy_axis_event>(event.jaxis);
        break;
      case SDL_JOYBALLMOTION:
        mData.emplace<joy_ball_event>(event.jball);
        break;
      case SDL_JOYHATMOTION:
        mData.emplace<joy_hat_event>(event.jhat);
        break;
      case SDL_JOYBUTTONDOWN:
      case SDL_JOYBUTTONUP:
        mData.emplace<joy_button_event>(event.jbutton);
        break;
      case SDL_JOYDEVICEADDED:
      case SDL_JOYDEVICEREMOVED:
        mData.emplace<joy_device_event>(event.jdevice);
        break;
      case SDL_CONTROLLERAXISMOTION:
        mData.emplace<controller_axis_event>(event.caxis);
        break;
      case SDL_CONTROLLERBUTTONDOWN:
      case SDL_CONTROLLERBUTTONUP:
        mData.emplace<controller_button_event>(event.cbutton);
        break;
      case SDL_CONTROLLERDEVICEADDED:
      case SDL_CONTROLLERDEVICEREMOVED:
      case SDL_CONTROLLERDEVICEREMAPPED:
        mData.emplace<controller_device_event>(event.cdevice);
        break;
      case SDL_FINGERDOWN:
      case SDL_FINGERUP:
      case SDL_FINGERMOTION:
        mData.emplace<touch_finger_event>(event.tfinger);
        break;
      case SDL_DOLLARGESTURE:
      case SDL_DOLLARRECORD:
        mData.emplace<dollar_gesture_event>(event.dgesture);
        break;
      case SDL_MULTIGESTURE:
        mData.emplace<multi_gesture_event>(event.mgesture);
        break;
      case SDL_CLIPBOARDUPDATE:
        break;
      case SDL_DROPFILE:
      case SDL_DROPTEXT:
      case SDL_DROPBEGIN:
      case SDL_DROPCOMPLETE:
        mData.emplace<drop_event>(event.drop);
        break;
      case SDL_AUDIODEVICEADDED:
      case SDL_AUDIODEVICEREMOVED:
        mData.emplace<audio_device_event>(event.adevice);
        break;
      case SDL_SENSORUPDATE:
        mData.emplace<sensor_event>(event.sensor);
        break;
      case SDL_RENDER_TARGETS_RESET:
      case SDL_RENDER_DEVICE_RESET:
        break;
      case SDL_USEREVENT:
        mData.emplace<user_event>(event.user);
        break;
      default:
        reset_state();
        break;
    }
  }
};
template <typename E>
class event_sink final
{
 public:
  using event_type = std::decay_t<E>;
  using signature_type = void(const event_type&);
  using function_type = std::function<signature_type>;
  void reset() noexcept { mFunction = nullptr; }
  template <typename T>
  void to(T&& callable)
  {
    static_assert(std::is_invocable_v<T, const event_type&>,
                  "Callable must be invocable with subscribed event!");
    mFunction = function_type{callable};
  }
  template <auto MemberFunc, typename Self>
  void to(Self* self)
  {
    static_assert(std::is_member_function_pointer_v<decltype(MemberFunc)>);
    static_assert(std::is_invocable_v<decltype(MemberFunc), Self*, const event_type&>,
                  "Member function must be invocable with subscribed event!");
    to(std::bind(MemberFunc, self, std::placeholders::_1));
  }
  template <auto Function>
  void to()
  {
    to(Function);
  }
  [[nodiscard]] auto function() -> function_type& { return mFunction; }
  [[nodiscard]] auto function() const -> const function_type& { return mFunction; }
 private:
  function_type mFunction;
};
template <typename... Events>
class event_dispatcher final
{
  static_assert((!std::is_const_v<Events> && ...));
  static_assert((!std::is_volatile_v<Events> && ...));
  static_assert((!std::is_reference_v<Events> && ...));
  static_assert((!std::is_pointer_v<Events> && ...));
  using sink_tuple = std::tuple<event_sink<Events>...>;
  template <typename Event>
  [[nodiscard]] constexpr static auto index_of() -> usize
  {
    using sink_type = event_sink<std::decay_t<Event>>;
    constexpr auto index = detail::tuple_type_index_v<sink_type, sink_tuple>;
    static_assert(index != -1, "Invalid event type!");
    return index;
  }
  template <typename Event>
  [[nodiscard]] auto get_sink() -> event_sink<Event>&
  {
    constexpr auto index = index_of<Event>();
    return std::get<index>(mSinks);
  }
  template <typename Event>
  [[nodiscard]] auto get_sink() const -> const event_sink<Event>&
  {
    constexpr auto index = index_of<Event>();
    return std::get<index>(mSinks);
  }
  template <typename Event>
  auto check_for() -> bool
  {
    if (const auto* event = mEvent.template try_get<Event>()) {
      auto& function = get_sink<Event>().function();
      if (function) {
        function(*event);
      }
      return true;
    }
    else {
      return false;
    }
  }
 public:
  void poll()
  {
    while (mEvent.poll()) {
      (check_for<Events>() || ...);
    }
  }
  template <typename Event>
  auto bind() -> event_sink<Event>&
  {
    static_assert((std::is_same_v<std::decay_t<Event>, Events> || ...),
                  "Cannot connect unsubscribed event! Make sure that the "
                  "event is provided as a class template parameter.");
    return get_sink<Event>();
  }
  void reset() noexcept { (bind<Events>().reset(), ...); }
  [[nodiscard]] auto active_count() const -> usize
  {
    return (0u + ... + (get_sink<Events>().function() ? 1u : 0u));
  }
  [[nodiscard]] constexpr static auto size() noexcept -> usize { return sizeof...(Events); }
 private:
  cen::event_handler mEvent;
  sink_tuple mSinks;
};
template <typename... E>
[[nodiscard]] auto to_string(const event_dispatcher<E...>& dispatcher) -> std::string
{
  return "event_dispatcher(size: " + std::to_string(dispatcher.size()) +
         ", #active: " + std::to_string(dispatcher.active_count()) + ")";
}
template <typename... E>
auto operator<<(std::ostream& stream, const event_dispatcher<E...>& dispatcher)
    -> std::ostream&
{
  return stream << to_string(dispatcher);
}
}
namespace cen {
enum class file_type : unsigned
{
  unknown = SDL_RWOPS_UNKNOWN,
  win = SDL_RWOPS_WINFILE,
  std = SDL_RWOPS_STDFILE,
  jni = SDL_RWOPS_JNIFILE,
  memory = SDL_RWOPS_MEMORY,
  memory_ro = SDL_RWOPS_MEMORY_RO
};
[[nodiscard]] constexpr auto to_string(const file_type type) -> std::string_view
{
  switch (type) {
    case file_type::unknown:
      return "unknown";
    case file_type::win:
      return "win";
    case file_type::std:
      return "std";
    case file_type::jni:
      return "jni";
    case file_type::memory:
      return "memory";
    case file_type::memory_ro:
      return "memory_ro";
    default:
      throw exception{"Did not recognize file type!"};
  }
}
inline auto operator<<(std::ostream& stream, const file_type type) -> std::ostream&
{
  return stream << to_string(type);
}
enum class file_mode
{
  r,
  rb,
  w,
  wb,
  a,
  ab,
  rx,
  rbx,
  wx,
  wbx,
  ax,
  abx,
};
[[nodiscard]] constexpr auto to_string(const file_mode mode) -> std::string_view
{
  switch (mode) {
    case file_mode::r:
      return "r";
    case file_mode::rb:
      return "rb";
    case file_mode::w:
      return "w";
    case file_mode::wb:
      return "wb";
    case file_mode::a:
      return "a";
    case file_mode::ab:
      return "ab";
    case file_mode::rx:
      return "rx";
    case file_mode::rbx:
      return "rbx";
    case file_mode::wx:
      return "wx";
    case file_mode::wbx:
      return "wbx";
    case file_mode::ax:
      return "ax";
    case file_mode::abx:
      return "abx";
    default:
      throw exception{"Did not recognize file mode!"};
  }
}
inline auto operator<<(std::ostream& stream, const file_mode mode) -> std::ostream&
{
  return stream << to_string(mode);
}
enum class seek_mode
{
  from_beginning = RW_SEEK_SET,
  relative_to_current = RW_SEEK_CUR,
  relative_to_end = RW_SEEK_END
};
[[nodiscard]] constexpr auto to_string(const seek_mode mode) -> std::string_view
{
  switch (mode) {
    case seek_mode::from_beginning:
      return "from_beginning";
    case seek_mode::relative_to_current:
      return "relative_to_current";
    case seek_mode::relative_to_end:
      return "relative_to_end";
    default:
      throw exception{"Did not recognize seek mode!"};
  }
}
inline auto operator<<(std::ostream& stream, const seek_mode mode) -> std::ostream&
{
  return stream << to_string(mode);
}
class file final
{
 public:
  using size_type = usize;
  explicit file(SDL_RWops* context) noexcept : mContext{context} {}
  file(const char* path, const file_mode mode) noexcept
      : mContext{SDL_RWFromFile(path, stringify(mode))}
  {}
  file(const std::string& path, const file_mode mode) noexcept : file{path.c_str(), mode} {}
  template <typename T>
  auto write(const T* data, const size_type count) noexcept -> size_type
  {
    assert(mContext);
    return SDL_RWwrite(this->data(), data, sizeof(T), count);
  }
  template <typename T, size_type Size>
  auto write(const T (&data)[Size]) noexcept -> size_type
  {
    return write(data, Size);
  }
  template <typename Container>
  auto write(const Container& container) noexcept(noexcept(container.data()) &&
                                                  noexcept(container.size()))
      -> size_type
  {
    return write(container.data(), container.size());
  }
  auto write_byte(const uint8 value) noexcept -> result
  {
    assert(mContext);
    return SDL_WriteU8(data(), value) == 1;
  }
  auto write_native_as_little_endian(const uint16 value) noexcept -> result
  {
    assert(mContext);
    return SDL_WriteLE16(data(), value) == 1;
  }
  auto write_native_as_little_endian(const uint32 value) noexcept -> result
  {
    assert(mContext);
    return SDL_WriteLE32(data(), value) == 1;
  }
  auto write_native_as_little_endian(const uint64 value) noexcept -> result
  {
    assert(mContext);
    return SDL_WriteLE64(data(), value) == 1;
  }
  auto write_native_as_big_endian(const uint16 value) noexcept -> result
  {
    assert(mContext);
    return SDL_WriteBE16(data(), value) == 1;
  }
  auto write_native_as_big_endian(const uint32 value) noexcept -> result
  {
    assert(mContext);
    return SDL_WriteBE32(data(), value) == 1;
  }
  auto write_native_as_big_endian(const uint64 value) noexcept -> result
  {
    assert(mContext);
    return SDL_WriteBE64(data(), value) == 1;
  }
  template <typename T>
  auto read_to(T* data, const size_type maxCount) noexcept -> size_type
  {
    assert(data);
    assert(mContext);
    return SDL_RWread(this->data(), data, sizeof(T), maxCount);
  }
  template <typename T, size_type Size>
  auto read_to(bounded_array_ref<T, Size> data) noexcept -> size_type
  {
    return read_to(data, Size);
  }
  template <typename Container>
  auto read_to(Container& container) noexcept(noexcept(container.data()) &&
                                              noexcept(container.size()))
      -> size_type
  {
    return read_to(container.data(), container.size());
  }
  template <typename T>
  auto read() noexcept(noexcept(T{})) -> T
  {
    T value{};
    read_to(&value, 1);
    return value;
  }
  auto read_byte() noexcept -> uint8
  {
    assert(mContext);
    return SDL_ReadU8(data());
  }
  auto read_little_endian_u16() noexcept -> uint16
  {
    assert(mContext);
    return SDL_ReadLE16(data());
  }
  auto read_little_endian_u32() noexcept -> uint32
  {
    assert(mContext);
    return SDL_ReadLE32(data());
  }
  auto read_little_endian_u64() noexcept -> uint64
  {
    assert(mContext);
    return SDL_ReadLE64(data());
  }
  auto read_big_endian_u16() noexcept -> uint16
  {
    assert(mContext);
    return SDL_ReadBE16(data());
  }
  auto read_big_endian_u32() noexcept -> uint32
  {
    assert(mContext);
    return SDL_ReadBE32(data());
  }
  auto read_big_endian_u64() noexcept -> uint64
  {
    assert(mContext);
    return SDL_ReadBE64(data());
  }
  [[nodiscard]] auto seek(const int64 offset, const seek_mode mode) noexcept -> maybe<int64>
  {
    assert(mContext);
    const auto result = SDL_RWseek(data(), offset, to_underlying(mode));
    if (result != -1) {
      return result;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto is_png() const noexcept -> bool { return IMG_isPNG(data()) == 1; }
  [[nodiscard]] auto is_ico() const noexcept -> bool { return IMG_isICO(data()) == 1; }
  [[nodiscard]] auto is_jpg() const noexcept -> bool { return IMG_isJPG(data()) == 1; }
  [[nodiscard]] auto is_bmp() const noexcept -> bool { return IMG_isBMP(data()) == 1; }
  [[nodiscard]] auto is_gif() const noexcept -> bool { return IMG_isGIF(data()) == 1; }
  [[nodiscard]] auto is_svg() const noexcept -> bool { return IMG_isSVG(data()) == 1; }
  [[nodiscard]] auto is_webp() const noexcept -> bool { return IMG_isWEBP(data()) == 1; }
  [[nodiscard]] auto is_tif() const noexcept -> bool { return IMG_isTIF(data()) == 1; }
  [[nodiscard]] auto is_pnm() const noexcept -> bool { return IMG_isPNM(data()) == 1; }
  [[nodiscard]] auto is_pcx() const noexcept -> bool { return IMG_isPCX(data()) == 1; }
  [[nodiscard]] auto is_lbm() const noexcept -> bool { return IMG_isLBM(data()) == 1; }
  [[nodiscard]] auto is_cur() const noexcept -> bool { return IMG_isCUR(data()) == 1; }
  [[nodiscard]] auto is_xcf() const noexcept -> bool { return IMG_isXCF(data()) == 1; }
  [[nodiscard]] auto is_xpm() const noexcept -> bool { return IMG_isXPM(data()) == 1; }
  [[nodiscard]] auto is_xv() const noexcept -> bool { return IMG_isXV(data()) == 1; }
  [[nodiscard]] auto offset() const noexcept -> int64
  {
    assert(mContext);
    return SDL_RWtell(data());
  }
  [[nodiscard]] auto type() const noexcept -> file_type
  {
    assert(mContext);
    return static_cast<file_type>(mContext->type);
  }
  [[nodiscard]] auto size() const noexcept -> maybe<usize>
  {
    assert(mContext);
    const auto result = SDL_RWsize(data());
    if (result != -1) {
      return result;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto data() const noexcept -> SDL_RWops* { return mContext.get(); }
  [[nodiscard]] auto is_ok() const noexcept -> bool { return mContext != nullptr; }
  explicit operator bool() const noexcept { return is_ok(); }
 private:
  managed_ptr<SDL_RWops> mContext;
  [[nodiscard]] static auto stringify(const file_mode mode) noexcept -> const char*
  {
    switch (mode) {
      default:
        assert(false);
      case file_mode::r:
        return "r";
      case file_mode::rb:
        return "rb";
      case file_mode::w:
        return "w";
      case file_mode::wb:
        return "wb";
      case file_mode::a:
        return "a";
      case file_mode::ab:
        return "ab";
      case file_mode::rx:
        return "r+";
      case file_mode::rbx:
        return "rb+";
      case file_mode::wx:
        return "w+";
      case file_mode::wbx:
        return "wb+";
      case file_mode::ax:
        return "a+";
      case file_mode::abx:
        return "ab+";
    }
  }
};
[[nodiscard]] inline auto base_path() -> sdl_string { return sdl_string{SDL_GetBasePath()}; }
[[nodiscard]] inline auto preferred_path(const char* org, const char* app) -> sdl_string
{
  assert(org);
  assert(app);
  return sdl_string{SDL_GetPrefPath(org, app)};
}
[[nodiscard]] inline auto preferred_path(const std::string& org, const std::string& app)
    -> sdl_string
{
  return preferred_path(org.c_str(), app.c_str());
}
}
namespace cen {
enum class font_hint
{
  normal = TTF_HINTING_NORMAL,
  light = TTF_HINTING_LIGHT,
  mono = TTF_HINTING_MONO,
  none = TTF_HINTING_NONE
};
[[nodiscard]] constexpr auto to_string(const font_hint hint) -> std::string_view
{
  switch (hint) {
    case font_hint::normal:
      return "normal";
    case font_hint::light:
      return "light";
    case font_hint::mono:
      return "mono";
    case font_hint::none:
      return "none";
    default:
      throw exception{"Did not recognize font hint!"};
  }
}
inline auto operator<<(std::ostream& stream, const font_hint hint) -> std::ostream&
{
  return stream << to_string(hint);
}
struct glyph_metrics final
{
  int min_x{};
  int min_y{};
  int max_x{};
  int max_y{};
  int advance{};
};
class font final
{
 public:
  font(const char* file, const int size) : mSize{size}
  {
    assert(file);
    if (mSize <= 0) {
      throw exception{"Bad font size!"};
    }
    mFont.reset(TTF_OpenFont(file, mSize));
    if (!mFont) {
      throw ttf_error{};
    }
  }
  font(const std::string& file, const int size) : font{file.c_str(), size} {}
  void reset_style() noexcept { TTF_SetFontStyle(mFont.get(), TTF_STYLE_NORMAL); }
  void set_bold(const bool bold) noexcept
  {
    if (bold) {
      add_style(TTF_STYLE_BOLD);
    }
    else {
      remove_style(TTF_STYLE_BOLD);
    }
  }
  void set_italic(const bool italic) noexcept
  {
    if (italic) {
      add_style(TTF_STYLE_ITALIC);
    }
    else {
      remove_style(TTF_STYLE_ITALIC);
    }
  }
  void set_underlined(const bool underlined) noexcept
  {
    if (underlined) {
      add_style(TTF_STYLE_UNDERLINE);
    }
    else {
      remove_style(TTF_STYLE_UNDERLINE);
    }
  }
  void set_strikethrough(const bool strikethrough) noexcept
  {
    if (strikethrough) {
      add_style(TTF_STYLE_STRIKETHROUGH);
    }
    else {
      remove_style(TTF_STYLE_STRIKETHROUGH);
    }
  }
  void set_kerning(const bool kerning) noexcept
  {
    TTF_SetFontKerning(mFont.get(), kerning ? 1 : 0);
  }
  void set_outline(const int outline) noexcept
  {
    TTF_SetFontOutline(mFont.get(), detail::max(outline, 0));
  }
  void set_hinting(const font_hint hint) noexcept
  {
    TTF_SetFontHinting(mFont.get(), to_underlying(hint));
  }
  [[nodiscard]] auto is_bold() const noexcept -> bool
  {
    return TTF_GetFontStyle(mFont.get()) & TTF_STYLE_BOLD;
  }
  [[nodiscard]] auto is_italic() const noexcept -> bool
  {
    return TTF_GetFontStyle(mFont.get()) & TTF_STYLE_ITALIC;
  }
  [[nodiscard]] auto is_underlined() const noexcept -> bool
  {
    return TTF_GetFontStyle(mFont.get()) & TTF_STYLE_UNDERLINE;
  }
  [[nodiscard]] auto is_strikethrough() const noexcept -> bool
  {
    return TTF_GetFontStyle(mFont.get()) & TTF_STYLE_STRIKETHROUGH;
  }
  [[nodiscard]] auto is_outlined() const noexcept -> bool { return outline() != 0; }
  [[nodiscard]] auto has_kerning() const noexcept -> bool
  {
    return TTF_GetFontKerning(mFont.get());
  }
  [[nodiscard]] auto outline() const noexcept -> int
  {
    return TTF_GetFontOutline(mFont.get());
  }
  [[nodiscard]] auto hinting() const noexcept -> font_hint
  {
    return static_cast<font_hint>(TTF_GetFontHinting(mFont.get()));
  }
  [[nodiscard]] auto is_fixed_width() const noexcept -> bool
  {
    return TTF_FontFaceIsFixedWidth(mFont.get());
  }
  [[nodiscard]] auto height() const noexcept -> int { return TTF_FontHeight(mFont.get()); }
  [[nodiscard]] auto descent() const noexcept -> int { return TTF_FontDescent(mFont.get()); }
  [[nodiscard]] auto ascent() const noexcept -> int { return TTF_FontAscent(mFont.get()); }
  [[nodiscard]] auto line_skip() const noexcept -> int
  {
    return TTF_FontLineSkip(mFont.get());
  }
  [[nodiscard]] auto face_count() const noexcept -> int
  {
    return static_cast<int>(TTF_FontFaces(mFont.get()));
  }
  [[nodiscard]] auto family_name() const noexcept -> const char*
  {
    return TTF_FontFaceFamilyName(mFont.get());
  }
  [[nodiscard]] auto style_name() const noexcept -> const char*
  {
    return TTF_FontFaceStyleName(mFont.get());
  }
  [[nodiscard]] auto size() const noexcept -> int { return mSize; }
  [[nodiscard]] auto get_kerning(const unicode_t previous,
                                 const unicode_t current) const noexcept -> int
  {
    return TTF_GetFontKerningSizeGlyphs(mFont.get(), previous, current);
  }
  [[nodiscard]] auto is_glyph_provided(const unicode_t glyph) const noexcept -> bool
  {
    return TTF_GlyphIsProvided(mFont.get(), glyph);
  }
  [[nodiscard]] auto get_metrics(const unicode_t glyph) const noexcept -> maybe<glyph_metrics>
  {
    glyph_metrics metrics;
    if (TTF_GlyphMetrics(mFont.get(),
                         glyph,
                         &metrics.min_x,
                         &metrics.max_x,
                         &metrics.min_y,
                         &metrics.max_y,
                         &metrics.advance) != -1) {
      return metrics;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto render_solid_glyph(const unicode_t glyph, const color& fg) const
      -> surface
  {
    return surface{TTF_RenderGlyph_Solid(get(), glyph, fg.get())};
  }
  [[nodiscard]] auto render_shaded_glyph(const unicode_t glyph,
                                         const color& fg,
                                         const color& bg) const -> surface
  {
    return surface{TTF_RenderGlyph_Shaded(get(), glyph, fg.get(), bg.get())};
  }
  [[nodiscard]] auto render_blended_glyph(const unicode_t glyph, const color& fg) const
      -> surface
  {
    return surface{TTF_RenderGlyph_Blended(get(), glyph, fg.get())};
  }
  [[nodiscard]] auto calc_size(const char* str) const noexcept -> maybe<iarea>
  {
    assert(str);
    iarea size{};
    if (TTF_SizeText(mFont.get(), str, &size.width, &size.height) != -1) {
      return size;
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto calc_size(const std::string& str) const noexcept -> maybe<iarea>
  {
    return calc_size(str.c_str());
  }
  [[nodiscard]] auto render_blended(const char* str, const color& fg) const -> surface
  {
    assert(str);
    return surface{TTF_RenderText_Blended(get(), str, fg.get())};
  }
  [[nodiscard]] auto render_blended_utf8(const char* str, const color& fg) const -> surface
  {
    assert(str);
    return surface{TTF_RenderUTF8_Blended(get(), str, fg.get())};
  }
  [[nodiscard]] auto render_blended_uni(const unicode_string& str, const color& fg) const
      -> surface
  {
    return surface{TTF_RenderUNICODE_Blended(get(), str.data(), fg.get())};
  }
  [[nodiscard]] auto render_solid(const char* str, const color& fg) const -> surface
  {
    assert(str);
    return surface{TTF_RenderText_Solid(get(), str, fg.get())};
  }
  [[nodiscard]] auto render_solid_utf8(const char* str, const color& fg) const -> surface
  {
    assert(str);
    return surface{TTF_RenderUTF8_Solid(get(), str, fg.get())};
  }
  [[nodiscard]] auto render_solid_uni(const unicode_string& str, const color& fg) const
      -> surface
  {
    return surface{TTF_RenderUNICODE_Solid(get(), str.data(), fg.get())};
  }
  [[nodiscard]] auto render_shaded(const char* str, const color& fg, const color& bg) const
      -> surface
  {
    assert(str);
    return surface{TTF_RenderText_Shaded(get(), str, fg.get(), bg.get())};
  }
  [[nodiscard]] auto render_shaded_utf8(const char* str,
                                        const color& fg,
                                        const color& bg) const -> surface
  {
    assert(str);
    return surface{TTF_RenderUTF8_Shaded(get(), str, fg.get(), bg.get())};
  }
  [[nodiscard]] auto render_shaded_uni(const unicode_string& str,
                                       const color& fg,
                                       const color& bg) const -> surface
  {
    return surface{TTF_RenderUNICODE_Shaded(get(), str.data(), fg.get(), bg.get())};
  }
  [[nodiscard]] auto render_blended_wrapped(const char* str,
                                            const color& fg,
                                            const uint32 wrap) const -> surface
  {
    assert(str);
    return surface{TTF_RenderText_Blended_Wrapped(get(), str, fg.get(), wrap)};
  }
  [[nodiscard]] auto render_blended_wrapped_utf8(const char* str,
                                                 const color& fg,
                                                 const uint32 wrap) const -> surface
  {
    assert(str);
    return surface{TTF_RenderUTF8_Blended_Wrapped(get(), str, fg.get(), wrap)};
  }
  [[nodiscard]] auto render_blended_wrapped_uni(const unicode_string& str,
                                                const color& fg,
                                                const uint32 wrap) const -> surface
  {
    return surface{TTF_RenderUNICODE_Blended_Wrapped(get(), str.data(), fg.get(), wrap)};
  }
  [[nodiscard]] auto get() const noexcept -> TTF_Font* { return mFont.get(); }
 private:
  managed_ptr<TTF_Font> mFont;
  int mSize{};
  void add_style(const int mask) noexcept
  {
    const auto style = TTF_GetFontStyle(mFont.get());
    TTF_SetFontStyle(mFont.get(), style | mask);
  }
  void remove_style(const int mask) noexcept
  {
    const auto style = TTF_GetFontStyle(mFont.get());
    TTF_SetFontStyle(mFont.get(), style & ~mask);
  }
};
[[nodiscard]] inline auto to_string(const font& font) -> std::string
{
  return "font(data: " + detail::address_of(font.get()) + ", name: '" +
         str_or_na(font.family_name()) + "', size: " + std::to_string(font.size()) + ")";
}
inline auto operator<<(std::ostream& stream, const font& font) -> std::ostream&
{
  return stream << to_string(font);
}
class font_cache final
{
 public:
  using id_type = usize;
  using size_type = usize;
  struct glyph_data final
  {
    texture glyph;
    glyph_metrics metrics;
  };
  font_cache(const char* file, const int size) : mFont{file, size} {}
  font_cache(const std::string& file, const int size) : mFont{file, size} {}
  explicit font_cache(font&& font) noexcept : mFont{std::move(font)} {}
  template <typename T>
  auto render_glyph(basic_renderer<T>& renderer, const unicode_t glyph, const ipoint& position)
      -> int
  {
    if (const auto* data = find_glyph(glyph)) {
      const auto& [texture, metrics] = *data;
      const auto outline = mFont.outline();
      const auto x = position.x() + metrics.min_x - outline;
      const auto y = position.y() - outline;
      renderer.render(texture, ipoint{x, y});
      return x + metrics.advance;
    }
    else {
      return position.x();
    }
  }
  template <typename T, typename String>
  void render_text(basic_renderer<T>& renderer, const String& str, ipoint position)
  {
    const auto originalX = position.x();
    const auto lineSkip = mFont.line_skip();
    for (const unicode_t glyph : str) {
      if (glyph == '\n') {
        position.set_x(originalX);
        position.set_y(position.y() + lineSkip);
      }
      else {
        const auto x = render_glyph(renderer, glyph, position);
        position.set_x(x);
      }
    }
  }
  template <typename T>
  auto store(basic_renderer<T>& renderer, const surface& surface) -> id_type
  {
    const auto id = mNextStringId;
    assert(mStrings.find(id) == mStrings.end());
    mStrings.try_emplace(id, renderer.make_texture(surface));
    ++mNextStringId;
    return id;
  }
  [[nodiscard]] auto find_string(const id_type id) const -> const texture*
  {
    if (const auto iter = mStrings.find(id); iter != mStrings.end()) {
      return &iter->second;
    }
    else {
      return nullptr;
    }
  }
  [[nodiscard]] auto has_string(const id_type id) const -> bool
  {
    return find_string(id) != nullptr;
  }
  [[nodiscard]] auto get_string(const id_type id) const -> const texture&
  {
    if (const auto* ptr = find_string(id)) {
      return *ptr;
    }
    else {
      throw exception{"Invalid font cache string identifier!"};
    }
  }
  template <typename T>
  void store_glyph(basic_renderer<T>& renderer, const unicode_t glyph)
  {
    if (has_glyph(glyph) || !mFont.is_glyph_provided(glyph)) {
      return;
    }
    glyph_data data{make_glyph_texture(renderer, glyph), mFont.get_metrics(glyph).value()};
    mGlyphs.try_emplace(glyph, std::move(data));
  }
  template <typename T>
  void store_glyphs(basic_renderer<T>& renderer, const unicode_t begin, const unicode_t end)
  {
    for (auto glyph = begin; glyph < end; ++glyph) {
      store_glyph(renderer, glyph);
    }
  }
  template <typename T>
  void store_basic_latin_glyphs(basic_renderer<T>& renderer)
  {
    store_glyphs(renderer, 0x20, 0x7F);
  }
  template <typename T>
  void store_latin1_supplement_glyphs(basic_renderer<T>& renderer)
  {
    store_glyphs(renderer, 0xA0, 0xFF + 0x1);
  }
  template <typename T>
  void store_latin1_glyphs(basic_renderer<T>& renderer)
  {
    store_basic_latin_glyphs(renderer);
    store_latin1_supplement_glyphs(renderer);
  }
  [[nodiscard]] auto find_glyph(const unicode_t glyph) const -> const glyph_data*
  {
    if (const auto it = mGlyphs.find(glyph); it != mGlyphs.end()) {
      return &it->second;
    }
    else {
      return nullptr;
    }
  }
  [[nodiscard]] auto has_glyph(const unicode_t glyph) const noexcept -> bool
  {
    return find_glyph(glyph) != nullptr;
  }
  [[nodiscard]] auto get_glyph(const unicode_t glyph) const -> const glyph_data&
  {
    if (const auto* ptr = find_glyph(glyph)) {
      return *ptr;
    }
    else {
      throw exception{"Invalid font cache glyph!"};
    }
  }
  [[nodiscard]] auto get_font() noexcept -> font& { return mFont; }
  [[nodiscard]] auto get_font() const noexcept -> const font& { return mFont; }
 private:
  font mFont;
  std::unordered_map<unicode_t, glyph_data> mGlyphs;
  std::unordered_map<id_type, texture> mStrings;
  id_type mNextStringId{1};
  template <typename T>
  [[nodiscard]] auto make_glyph_texture(basic_renderer<T>& renderer, const unicode_t glyph)
      -> texture
  {
    return renderer.make_texture(mFont.render_blended_glyph(glyph, renderer.get_color()));
  }
};
[[nodiscard]] inline auto to_string(const font_cache& cache) -> std::string
{
  const auto& font = cache.get_font();
  const auto* name = str_or_na(font.family_name());
  const auto size = font.size();
  using namespace std::string_literals;
  return "font_cache(font: '"s + name + "', size: " + std::to_string(size) + ")";
}
inline auto operator<<(std::ostream& stream, const font_cache& cache) -> std::ostream&
{
  return stream << to_string(cache);
}
namespace experimental {
class font_bundle final
{
 public:
  using id_type = usize;
  using size_type = usize;
  auto load_font(const char* path, const int size) -> id_type
  {
    assert(path);
    if (const auto id = get_id(path)) {
      mPools[*id].caches.try_emplace(size, font{path, size});
      return *id;
    }
    else {
      const auto newId = mNextFontId;
      auto& pack = mPools[newId];
      pack.path = path;
      pack.caches.try_emplace(size, font{path, size});
      ++mNextFontId;
      return newId;
    }
  }
  [[nodiscard]] auto contains(const id_type id) const -> bool
  {
    return mPools.find(id) != mPools.end();
  }
  [[nodiscard]] auto contains(const std::string_view path) const -> bool
  {
    return get_id(path).has_value();
  }
  [[nodiscard]] auto contains(const id_type id, const int size) const -> bool
  {
    if (const auto pack = mPools.find(id); pack != mPools.end()) {
      return pack->second.caches.find(size) != pack->second.caches.end();
    }
    else {
      return false;
    }
  }
  [[nodiscard]] auto at(const id_type id, const int size) -> font_cache&
  {
    if (const auto pool = mPools.find(id); pool != mPools.end()) {
      auto& caches = pool->second.caches;
      if (const auto cache = caches.find(size); cache != caches.end()) {
        return cache->second;
      }
      else {
        throw exception{"No loaded font of the requested size!"};
      }
    }
    else {
      throw exception{"Invalid font pool identifier!"};
    }
  }
  [[nodiscard]] auto at(const id_type id, const int size) const -> const font_cache&
  {
    return mPools.at(id).caches.at(size);
  }
  [[nodiscard]] auto get_font(const id_type id, const int size) -> font&
  {
    return at(id, size).get_font();
  }
  [[nodiscard]] auto get_font(const id_type id, const int size) const -> const font&
  {
    return at(id, size).get_font();
  }
  [[nodiscard]] auto font_count() const noexcept -> size_type
  {
    size_type count = 0;
    for (const auto& [id, pack] : mPools) {
      count += pack.caches.size();
    }
    return count;
  }
  [[nodiscard]] auto pool_count() const -> size_type { return mPools.size(); }
 private:
  struct font_pool final
  {
    std::string path;
    std::unordered_map<int, font_cache> caches;
  };
  std::unordered_map<id_type, font_pool> mPools;
  id_type mNextFontId{1};
  [[nodiscard]] auto get_id(const std::string_view path) const -> maybe<id_type>
  {
    for (const auto& [id, pack] : mPools) {
      if (!pack.caches.empty()) {
        if (pack.path == path) {
          return id;
        }
      }
    }
    return nothing;
  }
};
[[nodiscard]] inline auto to_string(const font_bundle& bundle) -> std::string
{
  return "font_bundle(#pools: " + std::to_string(bundle.pool_count()) +
         ", #fonts: " + std::to_string(bundle.font_count()) + ")";
}
inline auto operator<<(std::ostream& stream, const font_bundle& bundle) -> std::ostream&
{
  return stream << to_string(bundle);
}
}
}
namespace cen {
struct sdl_cfg final
{
  uint32 flags{SDL_INIT_EVERYTHING};
};
class sdl final
{
 public:
                           explicit sdl(const sdl_cfg& cfg = {})
  {
    if (SDL_Init(cfg.flags) < 0) {
      throw sdl_error{};
    }
  }
  ~sdl() noexcept { SDL_Quit(); }
};
struct img_cfg final
{
  int flags{IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF | IMG_INIT_WEBP};
};
class img final
{
 public:
                           explicit img(const img_cfg& cfg = {})
  {
    if (!IMG_Init(cfg.flags)) {
      throw img_error{};
    }
  }
  ~img() noexcept { IMG_Quit(); }
};
struct mix_cfg final
{
  int flags{MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC | MIX_INIT_MID | MIX_INIT_MOD |
            MIX_INIT_OPUS};
  int frequency{MIX_DEFAULT_FREQUENCY};
  uint16 format{MIX_DEFAULT_FORMAT};
  int channels{MIX_DEFAULT_CHANNELS};
  int chunk_size{4096};
};
class mix final
{
 public:
                           explicit mix(const mix_cfg& cfg = {})
  {
    if (!Mix_Init(cfg.flags)) {
      throw mix_error{};
    }
    if (Mix_OpenAudio(cfg.frequency, cfg.format, cfg.channels, cfg.chunk_size) == -1) {
      throw mix_error{};
    }
  }
  ~mix() noexcept
  {
    Mix_CloseAudio();
    Mix_Quit();
  }
};
class ttf final
{
 public:
                           ttf()
  {
    if (TTF_Init() == -1) {
      throw ttf_error{};
    }
  }
  ~ttf() noexcept { TTF_Quit(); }
};
}
namespace cen {
}
namespace cen {
enum class log_priority
{
  verbose = SDL_LOG_PRIORITY_VERBOSE,
  debug = SDL_LOG_PRIORITY_DEBUG,
  info = SDL_LOG_PRIORITY_INFO,
  warn = SDL_LOG_PRIORITY_WARN,
  error = SDL_LOG_PRIORITY_ERROR,
  critical = SDL_LOG_PRIORITY_CRITICAL,
};
[[nodiscard]] constexpr auto to_string(const log_priority priority) -> std::string_view
{
  switch (priority) {
    case log_priority::verbose:
      return "verbose";
    case log_priority::debug:
      return "debug";
    case log_priority::info:
      return "info";
    case log_priority::warn:
      return "warn";
    case log_priority::error:
      return "error";
    case log_priority::critical:
      return "critical";
    default:
      throw exception{"Did not recognize log priority!"};
  }
}
inline auto operator<<(std::ostream& stream, const log_priority priority) -> std::ostream&
{
  return stream << to_string(priority);
}
enum class log_category
{
  app = SDL_LOG_CATEGORY_APPLICATION,
  error = SDL_LOG_CATEGORY_ERROR,
  assert = SDL_LOG_CATEGORY_ASSERT,
  system = SDL_LOG_CATEGORY_SYSTEM,
  audio = SDL_LOG_CATEGORY_AUDIO,
  video = SDL_LOG_CATEGORY_VIDEO,
  render = SDL_LOG_CATEGORY_RENDER,
  input = SDL_LOG_CATEGORY_INPUT,
  test = SDL_LOG_CATEGORY_TEST,
  custom = SDL_LOG_CATEGORY_CUSTOM
};
[[nodiscard]] constexpr auto is_custom(const log_category category) noexcept -> bool
{
  return to_underlying(category) >= SDL_LOG_CATEGORY_CUSTOM;
}
[[nodiscard]] constexpr auto to_string(const log_category category) -> std::string_view
{
  if (is_custom(category)) {
    return "custom";
  }
  switch (category) {
    case log_category::app:
      return "app";
    case log_category::error:
      return "error";
    case log_category::assert:
      return "assert";
    case log_category::system:
      return "system";
    case log_category::audio:
      return "audio";
    case log_category::video:
      return "video";
    case log_category::render:
      return "render";
    case log_category::input:
      return "input";
    case log_category::test:
      return "test";
    case log_category::custom:
      return "custom";
    default:
      throw exception{"Did not recognize log category!"};
  }
}
inline auto operator<<(std::ostream& stream, const log_category category) -> std::ostream&
{
  return stream << to_string(category);
}
inline void reset_log_priorities() noexcept { SDL_LogResetPriorities(); }
inline void set_priority(const log_priority priority) noexcept
{
  const auto value = static_cast<SDL_LogPriority>(priority);
  SDL_LogSetAllPriority(value);
  SDL_LogSetPriority(SDL_LOG_CATEGORY_TEST, value);
}
inline void set_priority(const log_category category, const log_priority priority) noexcept
{
  SDL_LogSetPriority(to_underlying(category), static_cast<SDL_LogPriority>(priority));
}
[[nodiscard]] inline auto get_priority(const log_category category) noexcept -> log_priority
{
  return static_cast<log_priority>(SDL_LogGetPriority(to_underlying(category)));
}
[[nodiscard]] constexpr auto max_log_message_size() noexcept -> int
{
  return SDL_MAX_LOG_MESSAGE;
}
template <typename... Args>
void log(const log_priority priority,
         const log_category category,
         const char* fmt,
         Args&&... args) noexcept
{
  assert(fmt);
  SDL_LogMessage(static_cast<SDL_LogCategory>(category),
                 static_cast<SDL_LogPriority>(priority),
                 fmt,
                 std::forward<Args>(args)...);
}
template <typename... Args>
void log_verbose(const log_category category, const char* fmt, Args&&... args) noexcept
{
  log(log_priority::verbose, category, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void log_verbose(const char* fmt, Args&&... args) noexcept
{
  log_verbose(log_category::app, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void log_debug(const log_category category, const char* fmt, Args&&... args) noexcept
{
  log(log_priority::debug, category, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void log_debug(const char* fmt, Args&&... args) noexcept
{
  log_debug(log_category::app, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void log_info(const log_category category, const char* fmt, Args&&... args) noexcept
{
  log(log_priority::info, category, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void log_info(const char* fmt, Args&&... args) noexcept
{
  log_info(log_category::app, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void log_warn(const log_category category, const char* fmt, Args&&... args) noexcept
{
  log(log_priority::warn, category, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void log_warn(const char* fmt, Args&&... args) noexcept
{
  log_warn(log_category::app, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void log_error(const log_category category, const char* fmt, Args&&... args) noexcept
{
  log(log_priority::error, category, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void log_error(const char* fmt, Args&&... args) noexcept
{
  log_error(log_category::app, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void log_critical(const log_category category, const char* fmt, Args&&... args) noexcept
{
  log(log_priority::critical, category, fmt, std::forward<Args>(args)...);
}
template <typename... Args>
void log_critical(const char* fmt, Args&&... args) noexcept
{
  log_critical(log_category::app, fmt, std::forward<Args>(args)...);
}
}
namespace cen {
enum class message_box_type : uint32
{
  error = SDL_MESSAGEBOX_ERROR,
  warning = SDL_MESSAGEBOX_WARNING,
  information = SDL_MESSAGEBOX_INFORMATION
};
[[nodiscard]] constexpr auto to_string(const message_box_type type) -> std::string_view
{
  switch (type) {
    case message_box_type::error:
      return "error";
    case message_box_type::warning:
      return "warning";
    case message_box_type::information:
      return "information";
    default:
      throw exception{"Did not recognize message box type!"};
  }
}
inline auto operator<<(std::ostream& stream, const message_box_type type) -> std::ostream&
{
  return stream << to_string(type);
}
enum class message_box_button_order : uint32
{
  left_to_right,
  right_to_left
};
[[nodiscard]] constexpr auto to_string(const message_box_button_order order)
    -> std::string_view
{
  switch (order) {
    case message_box_button_order::left_to_right:
      return "left_to_right";
    case message_box_button_order::right_to_left:
      return "right_to_left";
    default:
      throw exception{"Did not recognize message box button order!"};
  }
}
inline auto operator<<(std::ostream& stream, const message_box_button_order order)
    -> std::ostream&
{
  return stream << to_string(order);
}
enum class message_box_color_type : int
{
  background = SDL_MESSAGEBOX_COLOR_BACKGROUND,
  text = SDL_MESSAGEBOX_COLOR_TEXT,
  button_border = SDL_MESSAGEBOX_COLOR_BUTTON_BORDER,
  button_background = SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND,
  button_selected = SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED
};
[[nodiscard]] constexpr auto to_string(const message_box_color_type type) -> std::string_view
{
  switch (type) {
    case message_box_color_type::background:
      return "background";
    case message_box_color_type::text:
      return "text";
    case message_box_color_type::button_border:
      return "button_border";
    case message_box_color_type::button_background:
      return "button_background";
    case message_box_color_type::button_selected:
      return "button_selected";
    default:
      throw exception{"Did not recognize message box color type!"};
  }
}
inline auto operator<<(std::ostream& stream, const message_box_color_type type)
    -> std::ostream&
{
  return stream << to_string(type);
}
class message_box_color_scheme final
{
 public:
  message_box_color_scheme() noexcept
  {
    set_color(message_box_color_type::background, colors::white);
    set_color(message_box_color_type::text, colors::white);
    set_color(message_box_color_type::button_background, colors::white);
    set_color(message_box_color_type::button_border, colors::white);
    set_color(message_box_color_type::button_selected, colors::white);
  }
  void set_color(const message_box_color_type id, const color& color) noexcept
  {
    assert(to_underlying(id) < 5);
    mScheme.colors[to_underlying(id)] = color.as_message_box_color();
  }
  [[nodiscard]] auto data() noexcept -> SDL_MessageBoxColorScheme* { return &mScheme; }
  [[nodiscard]] auto data() const noexcept -> const SDL_MessageBoxColorScheme*
  {
    return &mScheme;
  }
  [[nodiscard]] auto get() noexcept -> SDL_MessageBoxColorScheme& { return mScheme; }
  [[nodiscard]] auto get() const noexcept -> const SDL_MessageBoxColorScheme&
  {
    return mScheme;
  }
 private:
  SDL_MessageBoxColorScheme mScheme{};
};
class message_box final
{
 public:
  using button_id = int;
  enum button_flags : uint32
  {
    return_key_default = SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,
    escape_key_default = SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT
  };
  message_box() = default;
  message_box(std::string title, std::string message)
      : mTitle{std::move(title)}
      , mMessage{std::move(message)}
  {}
  template <typename T>
  static void show(const basic_window<T>& parent,
                   const char* title,
                   const char* message,
                   const message_box_type type = default_type(),
                   const message_box_button_order order = default_order())
  {
    show(parent.get(), title, message, type, order);
  }
  template <typename T>
  static void show(const basic_window<T>& parent,
                   const std::string& title,
                   const std::string& message,
                   const message_box_type type = default_type(),
                   const message_box_button_order order = default_order())
  {
    show(parent.get(), title.c_str(), message.c_str(), type, order);
  }
  static void show(const char* title,
                   const char* message,
                   const message_box_type type = default_type(),
                   const message_box_button_order order = default_order())
  {
    show(nullptr, title, message, type, order);
  }
  static void show(const std::string& title,
                   const std::string& message,
                   const message_box_type type = default_type(),
                   const message_box_button_order order = default_order())
  {
    show(nullptr, title.c_str(), message.c_str(), type, order);
  }
  template <typename T>
  auto show(const basic_window<T>& parent) -> maybe<button_id>
  {
    return show(parent.get());
  }
  auto show() -> maybe<button_id> { return show(nullptr); }
  void add_button(const button_id id,
                  std::string text,
                  const button_flags button = button_flags::return_key_default)
  {
    if (!has_button(id)) {
      mButtons.emplace_back(id, std::move(text), button);
    }
    else {
      throw exception{"Duplicate message box button ID!"};
    }
  }
  [[nodiscard]] auto has_button(const button_id id) const -> bool
  {
    return std::any_of(mButtons.begin(), mButtons.end(), [id](const button& button) noexcept {
      return button.id == id;
    });
  }
  void set_title(std::string title) { mTitle = std::move(title); }
  void set_message(std::string message) { mMessage = std::move(message); }
  void set_color_scheme(const message_box_color_scheme& scheme) noexcept
  {
    mColorScheme = scheme;
  }
  void set_type(const message_box_type type) noexcept { mType = type; }
  void set_button_order(const message_box_button_order order) noexcept
  {
    mButtonOrder = order;
  }
  [[nodiscard]] auto title() const -> const std::string& { return mTitle; }
  [[nodiscard]] auto message() const -> const std::string& { return mMessage; }
  [[nodiscard]] auto type() const noexcept -> message_box_type { return mType; }
  [[nodiscard]] auto button_order() const noexcept -> message_box_button_order
  {
    return mButtonOrder;
  }
  [[nodiscard]] constexpr static auto default_type() noexcept -> message_box_type
  {
    return message_box_type::information;
  }
  [[nodiscard]] constexpr static auto default_order() noexcept -> message_box_button_order
  {
    return message_box_button_order::left_to_right;
  }
 private:
  struct button final
  {
    button_id id;
    button_flags flags;
    std::string text;
    button(const button_id id, std::string text, const button_flags flags)
        : id{id}
        , flags{flags}
        , text{std::move(text)}
    {}
    [[nodiscard]] auto convert() const noexcept -> SDL_MessageBoxButtonData
    {
      SDL_MessageBoxButtonData result{};
      result.flags = to_underlying(flags);
      result.buttonid = id;
      result.text = text.c_str();
      return result;
    }
  };
  std::vector<button> mButtons;
  std::string mTitle{"Message box"};
  std::string mMessage;
  maybe<message_box_color_scheme> mColorScheme;
  message_box_type mType{default_type()};
  message_box_button_order mButtonOrder{default_order()};
  [[nodiscard]] constexpr static auto to_flags(const message_box_type type,
                                               const message_box_button_order order) noexcept
      -> uint32
  {
    return to_underlying(type) | to_underlying(order);
  }
  static void show(SDL_Window* parent,
                   const char* title,
                   const char* message,
                   const message_box_type type,
                   const message_box_button_order order)
  {
    assert(title);
    assert(message);
    const auto res = SDL_ShowSimpleMessageBox(to_flags(type, order), title, message, parent);
    if (res == -1) {
      throw sdl_error{};
    }
  }
  auto show(SDL_Window* parent) -> maybe<button_id>
  {
    SDL_MessageBoxData data{};
    data.window = parent;
    data.title = mTitle.c_str();
    data.message = mMessage.c_str();
    data.flags = to_flags(mType, mButtonOrder);
    data.colorScheme = mColorScheme ? mColorScheme->data() : nullptr;
    std::vector<SDL_MessageBoxButtonData> buttonData;
    buttonData.reserve(8);
    if (mButtons.empty()) {
      add_button(0, "OK", button_flags::return_key_default);
    }
    for (const auto& button : mButtons) {
      buttonData.emplace_back(button.convert());
    }
    data.buttons = buttonData.data();
    data.numbuttons = isize(buttonData);
    button_id button{-1};
    if (SDL_ShowMessageBox(&data, &button) == -1) {
      throw sdl_error{};
    }
    if (button != -1) {
      return button;
    }
    else {
      return nothing;
    }
  }
};
}
namespace cen {
enum class power_state
{
  unknown = SDL_POWERSTATE_UNKNOWN,
  on_battery = SDL_POWERSTATE_ON_BATTERY,
  no_battery = SDL_POWERSTATE_NO_BATTERY,
  charging = SDL_POWERSTATE_CHARGING,
  charged = SDL_POWERSTATE_CHARGED
};
[[nodiscard]] constexpr auto to_string(const power_state state) -> std::string_view
{
  switch (state) {
    case power_state::unknown:
      return "unknown";
    case power_state::on_battery:
      return "on_battery";
    case power_state::no_battery:
      return "no_battery";
    case power_state::charging:
      return "charging";
    case power_state::charged:
      return "charged";
    default:
      throw exception{"Did not recognize power state!"};
  }
}
inline auto operator<<(std::ostream& stream, const power_state state) -> std::ostream&
{
  return stream << to_string(state);
}
[[nodiscard]] inline auto battery_seconds() -> maybe<seconds<int>>
{
  int secondsLeft = -1;
  SDL_GetPowerInfo(&secondsLeft, nullptr);
  if (secondsLeft != -1) {
    return seconds<int>{secondsLeft};
  }
  else {
    return nothing;
  }
}
[[nodiscard]] inline auto battery_minutes() -> maybe<minutes<int>>
{
  if (const auto secondsLeft = battery_seconds()) {
    return std::chrono::duration_cast<minutes<int>>(*secondsLeft);
  }
  else {
    return nothing;
  }
}
[[nodiscard]] inline auto battery_percentage() noexcept -> maybe<int>
{
  int percentage = -1;
  SDL_GetPowerInfo(nullptr, &percentage);
  if (percentage != -1) {
    return percentage;
  }
  else {
    return nothing;
  }
}
[[nodiscard]] inline auto query_battery() noexcept -> power_state
{
  return static_cast<power_state>(SDL_GetPowerInfo(nullptr, nullptr));
}
[[nodiscard]] inline auto is_battery_available() noexcept -> bool
{
  const auto state = query_battery();
  return state != power_state::no_battery && state != power_state::unknown;
}
[[nodiscard]] inline auto is_battery_charging() noexcept -> bool
{
  return query_battery() == power_state::charging;
}
[[nodiscard]] inline auto is_battery_charged() noexcept -> bool
{
  return query_battery() == power_state::charged;
}
}
namespace cen {
inline constexpr bool on_linux = false;
inline constexpr bool on_apple = true;
inline constexpr bool on_win32 = false;
inline constexpr bool on_win64 = false;
inline constexpr bool on_windows = on_win32 || on_win64;
inline constexpr bool on_android = false;
enum class platform_id
{
  unknown,
  windows,
  macos,
  linux_os,
  ios,
  android
};
[[nodiscard]] inline auto to_string(const platform_id id) -> std::string_view
{
  switch (id) {
    case platform_id::unknown:
      return "unknown";
    case platform_id::windows:
      return "windows";
    case platform_id::macos:
      return "macos";
    case platform_id::linux_os:
      return "linux_os";
    case platform_id::ios:
      return "ios";
    case platform_id::android:
      return "android";
    default:
      throw exception{"Did not recognize platform!"};
  }
}
inline auto operator<<(std::ostream& stream, const platform_id id) -> std::ostream&
{
  return stream << to_string(id);
}
class shared_object final
{
 public:
  explicit shared_object(const char* object) : mObject{SDL_LoadObject(object)}
  {
    if (!mObject) {
      throw sdl_error{};
    }
  }
  explicit shared_object(const std::string& object) : shared_object{object.c_str()} {}
  template <typename T>
  [[nodiscard]] auto load_function(const char* name) const noexcept -> T*
  {
    assert(name);
    return reinterpret_cast<T*>(SDL_LoadFunction(mObject.get(), name));
  }
  template <typename T>
  [[nodiscard]] auto load_function(const std::string& name) const noexcept -> T*
  {
    return load_function<T>(name.c_str());
  }
 private:
  struct deleter final
  {
    void operator()(void* object) noexcept { SDL_UnloadObject(object); }
  };
  std::unique_ptr<void, deleter> mObject;
};
[[nodiscard]] inline auto platform_name() -> maybe<std::string>
{
  std::string name{SDL_GetPlatform()};
  if (name != "Unknown") {
    return name;
  }
  else {
    return nothing;
  }
}
[[nodiscard]] inline auto current_platform() noexcept -> platform_id
{
  const auto name = platform_name();
  if (name == "Windows") {
    return platform_id::windows;
  }
  else if (name == "Mac OS X") {
    return platform_id::macos;
  }
  else if (name == "Linux") {
    return platform_id::linux_os;
  }
  else if (name == "iOS") {
    return platform_id::ios;
  }
  else if (name == "Android") {
    return platform_id::android;
  }
  else {
    return platform_id::unknown;
  }
}
[[nodiscard]] inline auto is_windows() noexcept -> bool
{
  return current_platform() == platform_id::windows;
}
[[nodiscard]] inline auto is_macos() noexcept -> bool
{
  return current_platform() == platform_id::macos;
}
[[nodiscard]] inline auto is_linux() noexcept -> bool
{
  return current_platform() == platform_id::linux_os;
}
[[nodiscard]] inline auto is_ios() noexcept -> bool
{
  return current_platform() == platform_id::ios;
}
[[nodiscard]] inline auto is_android() noexcept -> bool
{
  return current_platform() == platform_id::android;
}
[[nodiscard]] inline auto is_tablet() noexcept -> bool { return SDL_IsTablet() == SDL_TRUE; }
[[nodiscard]] inline auto frequency() noexcept -> uint64
{
  return SDL_GetPerformanceFrequency();
}
[[nodiscard]] inline auto now() noexcept -> uint64 { return SDL_GetPerformanceCounter(); }
[[nodiscard]] inline auto now_in_seconds() noexcept(noexcept(seconds<double>{}))
    -> seconds<double>
{
  return seconds<double>{static_cast<double>(now()) / static_cast<double>(frequency())};
}
[[nodiscard, deprecated]] inline auto ticks32() noexcept(noexcept(u32ms{uint32{}})) -> u32ms
{
  return u32ms{SDL_GetTicks()};
}
[[nodiscard]] inline auto ram_mb() noexcept -> int { return SDL_GetSystemRAM(); }
[[nodiscard]] inline auto ram_gb() noexcept -> int { return ram_mb() / 1'000; }
inline auto set_clipboard(const char* text) noexcept -> result
{
  assert(text);
  return SDL_SetClipboardText(text) == 0;
}
inline auto set_clipboard(const std::string& text) noexcept -> result
{
  return set_clipboard(text.c_str());
}
[[nodiscard]] inline auto has_clipboard() noexcept -> bool { return SDL_HasClipboardText(); }
[[nodiscard]] inline auto get_clipboard() -> std::string
{
  const sdl_string text{SDL_GetClipboardText()};
  return text.copy();
}
}
namespace cen {
using touch_id = SDL_TouchID;
[[nodiscard]] constexpr auto touch_mouse_id() noexcept -> uint32 { return SDL_TOUCH_MOUSEID; }
[[nodiscard]] constexpr auto mouse_touch_id() noexcept -> touch_id
{
  return SDL_MOUSE_TOUCHID;
}
enum class touch_device_type
{
  invalid = SDL_TOUCH_DEVICE_INVALID,
  direct = SDL_TOUCH_DEVICE_DIRECT,
  indirect_absolute = SDL_TOUCH_DEVICE_INDIRECT_ABSOLUTE,
  indirect_relative = SDL_TOUCH_DEVICE_INDIRECT_RELATIVE
};
[[nodiscard]] constexpr auto to_string(const touch_device_type type) -> std::string_view
{
  switch (type) {
    case touch_device_type::invalid:
      return "invalid";
    case touch_device_type::direct:
      return "direct";
    case touch_device_type::indirect_absolute:
      return "indirect_absolute";
    case touch_device_type::indirect_relative:
      return "indirect_relative";
    default:
      throw exception{"Did not recognize touch device type!"};
  }
}
inline auto operator<<(std::ostream& stream, const touch_device_type type) -> std::ostream&
{
  return stream << to_string(type);
}
class finger final
{
 public:
  using id_type = SDL_FingerID;
  [[nodiscard]] static auto find(const touch_id id, const int index) noexcept -> maybe<finger>
  {
    if (const auto* data = SDL_GetTouchFinger(id, index)) {
      return finger{*data};
    }
    else {
      return nothing;
    }
  }
  [[nodiscard]] auto id() const noexcept -> id_type { return mFinger.id; }
  [[nodiscard]] auto x() const noexcept -> float { return mFinger.x; }
  [[nodiscard]] auto y() const noexcept -> float { return mFinger.y; }
  [[nodiscard]] auto pressure() const noexcept -> float { return mFinger.pressure; }
  [[nodiscard]] auto get() const noexcept -> const SDL_Finger& { return mFinger; }
 private:
  SDL_Finger mFinger{};
  explicit finger(const SDL_Finger& other) noexcept : mFinger{other} {}
};
[[nodiscard]] inline auto touch_device_count() noexcept -> int
{
  return SDL_GetNumTouchDevices();
}
[[nodiscard]] inline auto get_touch_device(const int index) noexcept -> maybe<touch_id>
{
  const auto device = SDL_GetTouchDevice(index);
  if (device != 0) {
    return device;
  }
  else {
    return nothing;
  }
}
[[nodiscard]] inline auto get_touch_type(const touch_id id) noexcept -> touch_device_type
{
  return static_cast<touch_device_type>(SDL_GetTouchDeviceType(id));
}
[[nodiscard]] inline auto get_touch_finger_count(const touch_id id) noexcept -> int
{
  return SDL_GetNumTouchFingers(id);
}
}
}
