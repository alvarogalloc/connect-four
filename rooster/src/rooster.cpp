module;
#include <functional>
#include <vector>
export module rooster;
import ginseng;

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

public:
  ginseng::database m_registry;
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
    m_gameflow = gameflow::running;
    auto game_entity = m_registry.create_entity();
    m_registry.add_component(game_entity, game_tag{});
    m_registry.add_component(game_entity, m_gameflow);
    m_hook_setup.publish(m_registry);
    while (m_registry.get_component<gameflow>(game_entity) == gameflow::running) { m_hook_systems.publish(m_registry); }
    m_hook_end.publish(m_registry);
  }
};

}// namespace rooster
