#include <centurion/event.hpp>
#include <centurion/initialization.hpp>
#include <centurion/math.hpp>
#include <centurion/render.hpp>
#include <centurion/window.hpp>
#include <chrono>
#include <random>
import rooster;

namespace component {

struct vec2f {
  float x;
  float y;
};
struct posvel {
  cen::fpoint pos;
  cen::fpoint vel;
};
struct circle {
  float radius;
  bool fill;
};
} // namespace component

template <typename T> T random(T min, T max) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  if constexpr (std::integral<T>) {
    std::uniform_int_distribution<T> dis(min, max);
    return dis(gen);
  } else if constexpr (std::floating_point<T>) {
    std::uniform_real_distribution<T> dis(min, max);
    return dis(gen);
  } else {
    static_assert(false,
                  "random only supports integral and floating point types");
  }
}

using my_events = cen::event_dispatcher<cen::quit_event>;

void startup(rooster::registry &reg) {

  my_events dispatcher;
  dispatcher.bind<cen::quit_event>().to([&reg](const cen::quit_event &event) {
    rooster::info("quit event");
    reg.ctx().get<rooster::gameflow &>() = rooster::gameflow::stop;
  });
  reg.ctx().emplace<my_events>(dispatcher);
}
void spawn_circle(rooster::registry &reg) {}

void camera_system(rooster::registry &reg) {}

void render_system(rooster::registry &reg) {
  auto &renderer = reg.ctx().get<cen::renderer_handle>();
  auto circle_view = reg.view<component::circle, component::posvel>();
  renderer.clear_with(cen::colors::cyan);
  renderer.set_color(cen::colors::red);
  for (auto ent : circle_view) {
    auto &circle = circle_view.get<component::circle>(ent);
    auto &[pos, vel] = circle_view.get<component::posvel>(ent);

    if (circle.fill)
      renderer.fill_circle(pos, circle.radius);
    else
      renderer.draw_circle(pos, circle.radius);
  }

  renderer.present();
}

void update_system(rooster::registry &reg) {
  auto &dispatcher = reg.ctx().get<my_events>();
  dispatcher.poll();
}
void moving_system(rooster::registry &reg) {
  auto view = reg.view<component::posvel>();
  // for (auto entity : view)
  // {
  //   auto &[pos, vel] = view.get<component::posvel>(entity);
  //   pos = pos + vel;
  // }
}
void bump_system(rooster::registry &reg) {}

int main(int, char *[]) {
  const cen::sdl sdl; // Init SDL
  const cen::img img; // Init SDL_image
  const cen::mix mix; // Init SDL_mixer
  const cen::ttf ttf; // Init SDL_ttf
  cen::window win{"Game", cen::iarea{800, 600}};
  cen::renderer renderer = win.make_renderer();
  rooster::game()
      .add_setup_callback([&](rooster::registry &reg) {
        win.show();
        reg.ctx().emplace<cen::window_handle>(win);
        reg.ctx().emplace<cen::renderer_handle>(renderer);
      })
      .add_setup_callback(startup)
      .add_setup_callback(spawn_circle)
      .add_end_callback(
          [](rooster::registry &) { rooster::info("adios alvaro"); })
      .add_system(update_system)
      .add_system(moving_system)
      .add_system(bump_system)
      .add_system(render_system)
      .run();
  win.hide();
  return 0;
}
