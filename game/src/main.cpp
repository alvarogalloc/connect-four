#include <iostream>
#include <fmt/format.h>
import board;
import rooster;
import random;
import ginseng;
import time_utils;
import centurion;

struct input_state
{
  cen::fpoint mouse_pos;
};
enum class turn_for { player1, player2 };
struct game_state
{
  turn_for turn;
  bool game_over;
};
using my_events =
  cen::event_dispatcher<cen::quit_event, cen::mouse_button_event, cen::mouse_motion_event, cen::keyboard_event>;
//
void startup(ginseng::database &reg)
{

  my_events dispatcher;

  reg.visit([&](ginseng::database::ent_id id, rooster::game_tag, rooster::gameflow &gameflow, board &b) {
    // game state
    reg.add_component(id, game_state{ turn_for::player1, false });

    // on quit
    dispatcher.bind<cen::quit_event>().to([&](const cen::quit_event &) { gameflow = rooster::gameflow::stop; });
    // on keypress

    // on mouse click (place piece)
    dispatcher.bind<cen::mouse_button_event>().to([&](const cen::mouse_button_event &event) {
      if (!event.pressed()) return;
      if (event.button() != cen::mouse_button::left) return;

      auto x = event.x() / 100;
      reg.visit([&](game_state &state) {
        b.put_piece(state.turn == turn_for::player1 ? piece::red : piece::yellow, x);
        state.game_over = b.check_winner(state.turn == turn_for::player1 ? piece::red : piece::yellow);
        if (state.game_over) return;
        state.turn = state.turn == turn_for::player1 ? turn_for::player2 : turn_for::player1;
      });
    });

    auto input_entity = reg.create_entity();
    reg.add_component(input_entity, input_state{});
    dispatcher.bind<cen::mouse_motion_event>().to([&](const cen::mouse_motion_event &event) {
      const cen::fpoint mouse_pos{ static_cast<float>(event.x()), static_cast<float>(event.y()) };
      reg.visit([&](input_state &state) { state.mouse_pos = mouse_pos; });
    });

    dispatcher.bind<cen::keyboard_event>().to([&](const cen::keyboard_event &e) {
      if (e.pressed() and e.key() == cen::keycodes::r)
        reg.visit([](rooster::game_tag, board &b, game_state &state) {
          b.reset();
          state.game_over = false;
        });
    });
    reg.add_component(id, dispatcher);
    reg.add_component(id, cen::font{ cen::base_path().copy() + "assets/BitPotion.ttf", 100 });
  });
}
void render_system(ginseng::database &reg)
{
  reg.visit([&](rooster::game_tag, cen::renderer_handle &ren, board &b, game_state &state, cen::font &font) {
    if (state.game_over) {
      ren.clear_with(cen::colors::blue);
      const auto font_text = ren.make_texture(font.render_blended(
        fmt::format("GAME OVER\n player {} wins", state.turn == turn_for::player1 ? "player1" : "player2").c_str(),
        cen::colors::white));
      ren.render(font_text, cen::ipoint{ 100, 100 });
    } else {
      ren.clear_with(cen::colors::white);
      b.draw(ren);
      reg.visit([&](input_state &state) { b.draw_placeholder(ren, state.mouse_pos); });
    }
    ren.present();
  });
}
void update_system(ginseng::database &reg)
{
  reg.visit([&](rooster::game_tag, my_events &dispatcher) { dispatcher.poll(); });
}

int main(int, char *[])
{
  auto start = now();
  const cen::sdl sdl;// Init SDL
  const cen::img img;// Init SDL_image
  const cen::ttf ttf;// Init SDL_ttf
  const cen::mix mix;// Init SDL_mixer
  cen::window win{ "Game", cen::iarea{ 700, 600 }, cen::window::window_flags::borderless };
  cen::renderer ren{ win.make_renderer() };
  ren.set_blend_mode(cen::blend_mode::blend);
  board b;
  win.show();
  try {
    rooster::game()
      .add_setup_callback([&](ginseng::database &reg) {
        reg.visit([&](ginseng::database::ent_id id, rooster::game_tag) {
          reg.add_component(id, cen::window_handle{ win });
          reg.add_component(id, cen::renderer_handle{ ren });
          reg.add_component(id, b);
        });
        std::cout << "setup took " << elapsed(start) << "ms\n";
      })
      .add_setup_callback(startup)
      .add_system(update_system)
      .add_system(render_system)
      .run();
  } catch (std::exception &e) {
    std::cerr << "exception: " << e.what() << "\n";
  }
  win.hide();
  return 0;
}
