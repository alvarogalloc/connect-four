module;
#include <vector>
#include <functional>

export module rooster:hooks;
export namespace rooster {
template<typename Type> class hook;
template<typename Ret, typename... Args> class hook<Ret(Args...)>
{
private:
  using callback_type = std::function<Ret(Args...)>;
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
}// namespace rooster
