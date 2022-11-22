#include "..\headers\consumable.h"

Consumable::Consumable() : Consumable("Steak", "a nice, juicy, useless steak", 0, 0, 0, 0) {}

Consumable::Consumable(string item_name, string item_desc, int healing, int strength_buff, int speed_buff, int duration) {
    name = item_name;
    desc = item_desc;
    heal = healing;
    str_b = strength_buff;
    move_b = speed_buff;
    dur = duration;
}

string Consumable::getName() const {
    return name;
}
string Consumable::getDesc() const {
    return desc;
}
int Consumable::getHeal() const {
    return heal;
}
int Consumable::getStrB() const {
    return str_b;
}
int Consumable::getMoveB() const {
    return move_b;
}
int Consumable::getDur() const {
    return dur;
}