module;
#include <array>
#include <cassert>
#include <fmt/core.h>
#include <ranges>// NOLINT
export module board;
import centurion;


export enum class piece { none, red, yellow };
export struct board
{
  constexpr static std::uint8_t rows = 6;
  constexpr static std::uint8_t columns = 7;
  constexpr static std::uint8_t cell_size = 100;
  board() noexcept;
  void draw(cen::renderer_handle renderer) const;
  void draw_grid(cen::renderer_handle renderer) const;
  void draw_pieces(cen::renderer_handle renderer) const;
  void put_piece(const piece piece, const std::uint8_t column);
  // when mouse over, draw a placeholder with a dimmed color
  void draw_placeholder(cen::renderer_handle renderer, const cen::fpoint mouse_pos) const;
  bool check_winner(const piece piece);
  void reset();


  std::array<piece, rows * columns> data;
};

module :private;

board::board() noexcept { std::fill(data.begin(), data.end(), piece::none); }
void board::draw_grid(cen::renderer_handle renderer) const
{
  renderer.set_color(cen::colors::black);
  for (auto i = 0; i < rows; ++i) {
    for (auto j = 0; j < columns; ++j) {
      const auto x = j * cell_size;
      const auto y = i * cell_size;
      renderer.draw_rect(cen::irect{ x, y, cell_size, cell_size });
    }
  }
}

void board::draw_pieces(cen::renderer_handle renderer) const
{
  for (auto &piece : std::ranges::views::filter(data, [](const auto &piece) { return piece != piece::none; })) {
    const auto index = std::distance(data.begin(), &piece);
    const auto x = (index % columns) * 100 + 50;
    const auto y = (index / columns) * 100 + 50;
    const auto color = piece == piece::red ? cen::colors::red : cen::colors::yellow;
    renderer.set_color(color);
    renderer.fill_circle(cen::ipoint{ static_cast<int>(x), static_cast<int>(y) }, 45);
  }
}

void board::draw(cen::renderer_handle renderer) const
{
  this->draw_grid(renderer);
  this->draw_pieces(renderer);
}

void board::put_piece(const piece piece, const std::uint8_t column)
{
  for (std::uint8_t i = rows - 1; i >= 0; --i) {
    const std::size_t index = i * columns + column;
    if (data[index] == piece::none) {
      data[index] = piece;
      break;
    }
  }
}

void board::draw_placeholder(cen::renderer_handle renderer, const cen::fpoint mouse_pos) const
{
  const auto [x, y] = mouse_pos.as_i().get();
  const auto column = x / cell_size;
  const auto row = y / cell_size;
  const auto index = static_cast<std::size_t>(row * columns + column);
  assert(index < data.size());
  const cen::color guide_color{ 0, 255, 55, 100 };
  const cen::irect guide_rect{ column * cell_size, row * cell_size, cell_size, cell_size };
  renderer.set_color(guide_color);
  renderer.fill_rect(guide_rect);
}

bool board::check_winner(const piece piece)
{
  std::uint8_t count_horizontal{ 0 };
  std::uint8_t count_vertical{ 0 };
  std::uint8_t count_diagonal{ 0 };
  enum class direction : std::uint8_t { horizontal, vertical, diagonal };
  auto get_neighbor = [](const int index, const direction dir, const bool forward) {
    switch (dir) {
    case direction::horizontal:
      return index + (forward ? 1 : -1);
    case direction::vertical:
      return index + (forward ? board::columns : -board::columns);
    case direction::diagonal:
      return index + (forward ? board::columns + 1 : -board::columns - 1);
    }
  };
  for (auto &el : data | std::views::filter([&](const auto &e) { return e == piece; })) {
    const auto index = std::distance(data.begin(), &el);
    for (auto dir : { direction::horizontal, direction::vertical, direction::diagonal }) {
      for (auto forward : { true, false }) {
        auto neighbor = get_neighbor(int(index), dir, forward);
        if (neighbor < 0 || size_t(neighbor) >= data.size()) { continue; }
        if (data[std::size_t(neighbor)] == piece) {
          switch (dir) {
          case direction::horizontal:
            count_horizontal++;
            break;
          case direction::vertical:
            count_vertical++;
            break;
          case direction::diagonal:
            count_diagonal++;
            break;
          }
        }
      }
    }
  }
  if (count_horizontal >= 3 || count_vertical >= 3 || count_diagonal >= 3) { return true; }

  return false;
}
void board::reset() { std::fill(data.begin(), data.end(), piece::none); }
