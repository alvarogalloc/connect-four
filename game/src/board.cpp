module;
#include <array>
#include <ranges>
export module board;
import centurion;

constexpr auto rows = 6;
constexpr auto columns = 7;

export enum class piece { none, red, yellow };
export struct board {
  board() noexcept;
  void draw(cen::renderer_handle renderer) const;
  void draw_grid(cen::renderer_handle renderer) const;
  void draw_pieces(cen::renderer_handle renderer) const;
  void put_piece(const piece piece, const int column);
  // when mouse over, draw a placeholder with a dimmed color
  void draw_placeholder(cen::renderer_handle renderer,
                        const cen::fpoint mouse_pos) const;

  std::array<piece, rows * columns> data;
};

module :private;

board::board() noexcept { std::fill(data.begin(), data.end(), piece::none); }
void board::draw_grid(cen::renderer_handle renderer) const {
  renderer.set_color(cen::colors::white);
  for (auto i = 0; i < rows; ++i) {
    for (auto j = 0; j < columns; ++j) {
      const auto x = j * 100;
      const auto y = i * 100;
      renderer.draw_rect(cen::irect{x, y, 100, 100});
    }
  }
}

void board::draw_pieces(cen::renderer_handle renderer) const {
  for (auto &piece : std::ranges::views::filter(
           data, [](const auto &piece) { return piece != piece::none; })) {
    const auto index = std::distance(data.begin(), &piece);
    const auto x = (index % columns) * 100 + 50;
    const auto y = (index / columns) * 100 + 50;
    const auto color =
        piece == piece::red ? cen::colors::red : cen::colors::yellow;
    renderer.set_color(color);
    renderer.fill_circle(cen::ipoint{static_cast<int>(x), static_cast<int>(y)},
                         45);
  }
}

void board::draw(cen::renderer_handle renderer) const {
  this->draw_grid(renderer);
  this->draw_pieces(renderer);
}

void board::put_piece(const piece piece, const int column) {
  for (auto i = rows - 1; i >= 0; --i) {
    const auto index = i * columns + column;
    if (data[index] == piece::none) {
      data[index] = piece;
      break;
    }
  }
}

void board::draw_placeholder(cen::renderer_handle renderer,
                             const cen::fpoint mouse_pos) const {
  const auto x = mouse_pos.x();
  const auto y = mouse_pos.y();
  const auto column = x / 100;
  const auto row = y / 100;
  const auto index = row * columns + column;
  if (data[index] != piece::none)
    return;
  const auto piece = data[index];
  const auto color =
      piece == piece::red ? cen::colors::red : cen::colors::yellow;
  renderer.set_color(color);
  renderer.fill_circle(mouse_pos.as_i(), 45);
}
