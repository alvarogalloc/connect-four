module;
#include <chrono>
#include <functional>
export module rooster:game;
import :hooks;
import ginseng;

using namespace std::chrono_literals;
export namespace rooster {

constexpr std::chrono::nanoseconds timestep(16ms);
using game_tag = ginseng::tag<struct game_tag_t>;

enum class gameflow { running, stop };
class game
{
private:
  using func_type = void(ginseng::database &);
  ginseng::database m_registry;
  hook<func_type> m_hook_setup;
  hook<func_type> m_hook_end;
  hook<func_type> m_hook_systems;
  gameflow m_gameflow;

public:
  using game_clock = std::chrono::steady_clock;
  game &add_setup_callback(std::function<func_type> func)
  {
    m_hook_setup.connect(func);
    return *this;
  }
  game &add_end_callback(std::function<func_type> func)
  {
    m_hook_end.connect(func);
    return *this;
  }
  game &add_system(std::function<func_type> func)
  {
    m_hook_systems.connect(func);
    return *this;
  }

  void run()
  {
    auto time_start = game_clock::now();
    m_gameflow = gameflow::running;
    auto game_entity = m_registry.create_entity();
    m_registry.add_component(game_entity, game_tag{});
    m_registry.add_component(game_entity, m_gameflow);
    m_hook_setup.publish(m_registry);

    while (m_registry.get_component<gameflow>(game_entity) == gameflow::running) {
      auto time_delta = std::chrono::duration_cast<std::chrono::duration<float>>(game_clock::now() - time_start);
      time_start = time_start = game_clock::now();
      m_hook_systems.publish(m_registry);
    }

    m_hook_end.publish(m_registry);
  }
};

}// namespace rooster
