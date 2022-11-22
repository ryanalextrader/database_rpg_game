#ifndef CONSUMABLE_H
#define CONSUMABLE_H

#include <string>
using std::string;

class Consumable {
private:
    string name;
    string desc;
    int heal;
    int str_b;
    int move_b;
    int dur;

public:
    Consumable();
    Consumable(string item_name, string item_desc, int healing, int strength_buff, int speed_buff, int duration);

    string getName() const;
    string getDesc() const;
    int getHeal() const;
    int getStrB() const;
    int getMoveB() const;
    int getDur() const;
};

#endif