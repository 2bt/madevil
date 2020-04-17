#include "enemy.hpp"
#include "game.hpp"


bool Enemy::can_see_hero() const {
    Box b = box();
    return m_game.can_see_hero(b.x + b.w * 0.5, b.y + b.h * 0.5);
}
