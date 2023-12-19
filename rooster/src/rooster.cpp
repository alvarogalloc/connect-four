module;
#include <functional>
#include <string>
#include <vector>
export module rooster;
export import :logging;
export import :time_utils;
export import ginseng;
export import centurion;

template<typename Sig, typename... Args> class hook
{
private:
  using callback_type = Sig;
  std::vector<callback_type> m_callbacks;

public:
  void connect(callback_type callback, bool front = false)
  {
    if (front)
      m_callbacks.insert(m_callbacks.begin(), callback);
    else
      m_callbacks.push_back(callback);
  }

  void publish(Args... args)
  {
    for (auto callback : m_callbacks) callback(args...);
  }
};
export namespace rooster {

using game_tag = ginseng::tag<struct game_tag_t>;

enum class gameflow { running, stop };
class game
{
private:
  using func_type = std::function<void(ginseng::database &)>;
  hook<func_type, ginseng::database &> m_hook_setup;
  hook<func_type, ginseng::database &> m_hook_end;
  hook<func_type, ginseng::database &> m_hook_systems;
  gameflow m_gameflow;
  ginseng::database m_registry;
  cen::window m_window;
  cen::renderer m_renderer;

  void init_window()
  {
    m_registry.visit([&](ginseng::database::ent_id id, rooster::game_tag) {
      m_registry.add_component(id, cen::window_handle{ m_window });
      m_registry.add_component(id, cen::renderer_handle{ m_renderer });
    });
    m_renderer.set_blend_mode(cen::blend_mode::blend);
    m_window.show();
  }

public:
  game(const std::string &title, const cen::iarea window_size)
    : m_window(title, window_size), m_renderer(m_window.make_renderer())
  {}
  ginseng::database &get_registry() { return m_registry; }
  cen::window_handle get_window() { return cen::window_handle{ m_window }; }
  cen::renderer_handle get_renderer() { return cen::renderer_handle{ m_renderer }; }
  game &add_setup_callback(func_type func)
  {
    m_hook_setup.connect(func);
    return *this;
  }
  game &add_end_callback(func_type func)
  {
    m_hook_end.connect(func);
    return *this;
  }
  game &add_system(func_type func)
  {
    m_hook_systems.connect(func);
    return *this;
  }

  void run()
  {
    auto start = now();
    m_gameflow = gameflow::running;
    auto game_entity = m_registry.create_entity();
    m_registry.add_component(game_entity, game_tag{});
    m_registry.add_component(game_entity, m_gameflow);
    init_window();
    m_hook_setup.publish(m_registry);
    logging::info("Time for startup {} ms", elapsed(start));
    while (m_registry.get_component<gameflow>(game_entity) == gameflow::running) { m_hook_systems.publish(m_registry); }
    m_hook_end.publish(m_registry);
    m_window.hide();
  }
};

}// namespace rooster
