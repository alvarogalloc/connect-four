module;
#include <array>
#include <cassert>
#include <fmt/core.h>
#include <ranges> // NOLINT
export module board;
import centurion;
import logging;


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
  void draw_placeholder(cen::renderer_handle renderer, const cen::ipoint mouse_pos) const;
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

void board::draw_placeholder(cen::renderer_handle renderer, const cen::ipoint mouse_pos) const
{
  const auto [x, y] = mouse_pos.get();
  const auto column = x / cell_size;
  const auto row = y / cell_size;
  assert(row * columns + column < data.size());
  const cen::color guide_color{ 0, 255, 55, 100 };
  const cen::irect guide_rect{ column * cell_size, row * cell_size, cell_size, cell_size };
  renderer.set_color(guide_color);
  renderer.fill_rect(guide_rect);
}

std::uint8_t search_left(std::size_t idx, std::span<piece> data, const piece piece)
{
  if (idx % board::columns != 0 && data[idx - 1] == piece) {
    return 1 + search_left(idx - 1, data, piece);
  } else
    return 0;
}

std::uint8_t search_right(std::size_t idx, std::span<piece> data, const piece piece)
{
  if (idx % board::columns != board::columns - 1 && data[idx + 1] == piece) {
    return 1 + search_right(idx + 1, data, piece);
  } else
    return 0;
}

std::uint8_t search_up(std::size_t idx, std::span<piece> data, const piece piece)
{
  if (idx / board::columns > 0 && data[idx - board::columns] == piece) {
    return 1 + search_up(idx - board::columns, data, piece);
  } else
    return 0;
}

std::uint8_t search_down(std::size_t idx, std::span<piece> data, const piece piece)
{
  if (idx / board::columns < board::columns - 1 && data[idx + board::columns] == piece) {
    return 1 + search_down(idx + board::columns, data, piece);
  } else
    return 0;
}

// four more to go: up-right, up-left, down-right, down-left
std::uint8_t search_up_right(std::size_t idx, std::span<piece> data, const piece piece){
  // check column is not 6
  if (idx % board::columns < 6 && data[idx - board::columns + 1] == piece) {
    return 1 + search_up_right(idx - board::columns + 1, data, piece);
  } else
    return 0;
}
std::uint8_t search_up_left(std::size_t idx, std::span<piece> data, const piece piece){
    // check column is not 0
  if (idx % board::columns > 0 && data[idx - board::columns - 1] == piece) {
    return 1 + search_up_left(idx - board::columns - 1, data, piece);
  } else
    return 0;
}

std::uint8_t search_down_right(std::size_t idx, std::span<piece> data, const piece piece){
    // check column is not 6
  if (idx % board::columns < 6 && data[idx + board::columns + 1] == piece) {
    return 1 + search_down_right(idx + board::columns + 1, data, piece);
  } else
    return 0;
}
std::uint8_t search_down_left(std::size_t idx, std::span<piece> data, const piece piece){
    // check column is not 0
  if (idx % board::columns > 0 && data[idx + board::columns - 1] == piece) {
    return 1 + search_down_right(idx + board::columns - 1, data, piece);
  } else
    return 0;
}

bool board::check_winner(const piece piece)
{

  for (auto &el : data | std::views::filter([&](const auto &e) { return e == piece; })) {
    const auto index = static_cast<std::size_t>(std::distance(data.begin(), &el));
    // we start in 1 because we are counting the selected one
    std::uint8_t count_horizontal = 1 + search_left(index, this->data, piece) + search_right(index, this->data, piece);
    std::uint8_t count_vertical = 1 + search_down(index, this->data, piece) + search_up(index, this->data, piece);
    std::uint8_t count_diagonal_down_to_up = 1 +search_down_left(index, data, piece)+ search_up_right(index, data, piece);
    std::uint8_t count_diagonal_up_to_down = 1 +search_up_left(index, data, piece)+ search_down_right(index, data, piece);

    if (count_horizontal >3 or count_vertical > 3 or count_diagonal_down_to_up > 3 or count_diagonal_up_to_down > 3){
      return true;
    }
  }


  return false;
}
void board::reset() { std::fill(data.begin(), data.end(), piece::none); }
