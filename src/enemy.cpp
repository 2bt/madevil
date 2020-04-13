#include "enemy.hpp"
#include "game.hpp"


bool Enemy::can_see_hero() const {
    return m_game.can_see_hero(m_x, m_y - 8);
}
