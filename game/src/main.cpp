#include <cassert>
import board;
import setup;
import rooster;
import ginseng;
import time_utils;
import centurion;
import logging;
#include <string_view>

auto make_text(cen::renderer_handle &ren,
  cen::font &font,
  const std::string_view text,
  const cen::color color = cen::colors::white)
{
  return ren.make_texture(font.render_blended(text.data(), color));
}
void render_system(ginseng::database &reg)
{
  reg.visit([&](ginseng::database::ent_id id, rooster::game_tag) {
    assert(reg.has_component<board>(id));
    assert(reg.has_component<cen::renderer_handle>(id));
    assert(reg.has_component<game_state>(id));
    auto &ren = reg.get_component<cen::renderer_handle>(id);
    auto &b = reg.get_component<board>(id);
    auto &state = reg.get_component<game_state>(id);
    const auto [w, h] = ren.output_size();

    auto draw_gameover_screen = [&]() {
      assert(reg.has_component<cen::font>(id));
      auto &font = reg.get_component<cen::font>(id);

      ren.set_color(cen::colors::blue);
      const cen::irect message_box{ (w - (w * 3 / 4)) / 2, (h - (h * 2 / 3)) / 2, w * 3 / 4, h * 2 / 3 };
      ren.fill_rect(message_box);
      ren.set_color(cen::colors::white);
      const auto gameover_text = make_text(ren, font, "GAME OVER");
      ren.render(gameover_text, message_box.position() + cen::ipoint{ 30, 30 });
      const auto text_winner = fmt::format("Player {} wins", state.turn == turn_for::player1 ? '1' : '2');
      const auto winner_text = make_text(ren, font, text_winner);
      ren.render(winner_text, message_box.position() + cen::ipoint{ 30, 80 });
      const std::string text_reset = "R to restart";
      const auto reset_text = make_text(ren, font, text_reset);
      ren.render(reset_text, message_box.position() + cen::ipoint{ 30, 180 });
    };
    ren.clear_with(cen::colors::white);
    b.draw(ren);
    if (state.game_over) {
      draw_gameover_screen();
    } else {
      reg.visit([&](input_state &state) { b.draw_placeholder(ren, state.mouse_pos); });
    }
    ren.present();
  });
}
void update_system(ginseng::database &reg)
{
  reg.visit([&](ginseng::database::ent_id id, rooster::game_tag) {
    auto on_quit = [&]() {
      auto &flow = reg.get_component<rooster::gameflow>(id);
      flow = rooster::gameflow::stop;
    };
    assert(reg.has_component<cen::event_handler>(id));
    assert(reg.has_component<game_state>(id));
    assert(reg.has_component<input_state>(id));
    assert(reg.has_component<board>(id));
    auto &handler = reg.get_component<cen::event_handler>(id);
    auto &state = reg.get_component<game_state>(id);
    auto &input = reg.get_component<input_state>(id);
    auto &b = reg.get_component<board>(id);

    auto on_mouse_down = [&](const auto &event) {
      if (!event.pressed()) return;
      if (event.button() != cen::mouse_button::left) return;

      auto x = event.x() / 100;
      b.put_piece(state.turn == turn_for::player1 ? piece::red : piece::yellow, static_cast<std::uint8_t>(x));
      state.game_over = b.check_winner(state.turn == turn_for::player1 ? piece::red : piece::yellow);
      if (state.game_over) return;
      state.turn = state.turn == turn_for::player1 ? turn_for::player2 : turn_for::player1;
    };

    auto on_mouse_move = [&](const auto &event) { input.mouse_pos = { event.x(), event.y() }; };

    auto on_key_down = [&](const auto &event) {
      if (event.pressed() and event.key() == cen::keycodes::r) {
        b.reset();
        state.game_over = false;
        state.turn = turn_for::player1;
      }
    };

    while (handler.poll()) {
      if (handler.is(cen::event_type::quit)) {
        on_quit();
      } else if (handler.is(cen::event_type::mouse_button_down)) {
        on_mouse_down(handler.get<cen::mouse_button_event>());
      } else if (handler.is(cen::event_type::mouse_motion)) {
        on_mouse_move(handler.get<cen::mouse_motion_event>());
      } else if (handler.is(cen::event_type::key_down)) {
        on_key_down(handler.get<cen::keyboard_event>());
      }
    }
  });
}

int main(int, char *[])
{
  const cen::sdl sdl;// Init SDL
  const cen::img img;// Init SDL_image
  const cen::ttf ttf;// Init SDL_ttf
  const cen::mix mix;// Init SDL_mixer

  rooster::game("Game", cen::iarea{ 700, 600 })
    .add_setup_callback(startup)
    .add_system(update_system)
    .add_system(render_system)
    .run();
  return 0;
}
