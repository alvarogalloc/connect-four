#include <iostream>
import board;
import rooster;
import random;
import time_utils;

// struct vec2f {
//   float x;
//   float y;
// };
// struct posvel {
//   cen::fpoint pos;
//   cen::fpoint vel;
// };
// struct circle {
//   float radius;
//   bool fill;
// };
//
struct input_state {
  cen::fpoint mouse_pos;
};
using my_events =
    cen::event_dispatcher<cen::quit_event, cen::mouse_button_event,
                          cen::mouse_motion_event>;
//
void startup(ginseng::database &reg) {

  my_events dispatcher;

  reg.visit([&](ginseng::database::ent_id id, rooster::game_tag,
                rooster::gameflow &gameflow, board &b) {
    dispatcher.bind<cen::quit_event>().to(
        [&](const cen::quit_event &) { gameflow = rooster::gameflow::stop; });

    dispatcher.bind<cen::mouse_button_event>().to(
        [&](const cen::mouse_button_event &event) {
          if (!event.pressed())
            return;
          if (event.button() == cen::mouse_button::left) {
            auto x = event.x() / 100;
            b.put_piece(piece::red, x);
          } else if (event.button() == cen::mouse_button::right) {
            auto x = event.x() / 100;
            b.put_piece(piece::yellow, x);
          }
        });
    auto input_entity = reg.create_entity();
    reg.add_component(input_entity, input_state{});
    dispatcher.bind<cen::mouse_motion_event>().to(
        [&](const cen::mouse_motion_event &event) {
          reg.visit([&](input_state &state) {
            state.mouse_pos = cen::fpoint{static_cast<float>(event.x()),
                                          static_cast<float>(event.y())};
          });
        });

    reg.add_component(id, dispatcher);
  });
}
void render_system(ginseng::database &reg) {
  reg.visit([&](rooster::game_tag, cen::renderer_handle &ren, board &b) {
    ren.clear_with(cen::colors::black);
    b.draw(ren);
    reg.visit(
        [&](input_state &state) { b.draw_placeholder(ren, state.mouse_pos); });
    ren.present();
  });
}
void update_system(ginseng::database &reg) {
  reg.visit(
      [&](rooster::game_tag, my_events &dispatcher) { dispatcher.poll(); });
}

int main(int, char *[]) {
  auto start = now();
  const cen::sdl sdl; // Init SDL
  const cen::img img; // Init SDL_image
  cen::window win{"Game", cen::iarea{700, 600}};
  cen::renderer ren{win.make_renderer()};
  board b;
  win.show();
  try {

    rooster::game()
        .add_setup_callback([&](ginseng::database &reg) {
          reg.visit([&](ginseng::database::ent_id id, rooster::game_tag) {
            reg.add_component(id, cen::window_handle{win});
            reg.add_component(id, cen::renderer_handle{ren});
            reg.add_component(id, b);
          });
          std::cout << "setup took " << elapsed(start) << "ms\n";
        })
        .add_setup_callback(startup)
        .add_system(update_system)
        .add_system(render_system)
        .run();
  } catch (std::exception &e) {
    std::cout << "exception: " << e.what() << "\n";
  }
  win.hide();
  return 0;
}
