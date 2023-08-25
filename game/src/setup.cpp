module;
#include <string>
export module setup;
import rooster;
import centurion;
import ginseng;
import board;

export
{
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

  void startup(ginseng::database & reg)
  {
    reg.visit([&](ginseng::database::ent_id id, rooster::game_tag) {
      // game state
      reg.add_component(id, game_state{ turn_for::player1, false });
      reg.add_component(id, input_state{ cen::fpoint{ 0.f, 0.f } });
      reg.add_component(id, board{});
      reg.add_component(id, cen::event_handler{});
      const auto path = cen::base_path().copy() + "assets/BitPotion.ttf";
      reg.add_component(id, cen::font{ path, 100 });
    });
  }
}
