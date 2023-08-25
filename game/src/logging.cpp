module;
#include <spdlog/spdlog.h>

export module logging;
export namespace fmt {
  using fmt::format;
}

export namespace logging {
  using spdlog::debug;
  using spdlog::error;
  using spdlog::info;
  using spdlog::warn;
}// namespace gallo

