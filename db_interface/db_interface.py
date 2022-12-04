import sys
import random
import pymysql

def get_list_monsters(save_id, num_monsters, max_mons_types, boss_fight):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT m.name, m.description, m.sight_range, m.atk_strength, m.atk_var, m.atk_range, m.move, m.acc, m.acc_decay, m.symbol, m.max_hp, m.default_beh, m.blind_beh, m.provoked_beh, m.is_boss "
    sql += "FROM monster m, "
    sql += "(SELECT s.level, ma.room_theme "
    sql += "FROM save s join map ma ON s.on_map = ma.id "
    sql += "WHERE s.id = " + str(save_id) + ") lt "
    sql += "WHERE m.room_theme = lt.room_theme AND m.start_floor <= lt.level AND m.end_floor >= lt.level AND NOT m.is_boss "
    sql += "ORDER BY Rand() "
    sql += "LIMIT " + str(max_mons_types)

    res = crsr.execute(sql) #execute the query
    for row in crsr:
        row_string = ''
        for att in row:
            row_string += str(att) + ';'
        query.append(row_string)

    while len(query) < num_monsters:
        query.append(random.choice(query))

    if boss_fight:
        sql = "SELECT m.name, m.description, m.sight_range, m.atk_strength, m.atk_var, m.atk_range, m.move, m.acc, m.acc_decay, m.symbol, m.max_hp, m.default_beh, m.blind_beh, m.provoked_beh, m.is_boss "
        sql += "FROM monster m, "
        sql += "(SELECT s.level, ma.room_theme "
        sql += "FROM save s join map ma ON s.on_map = ma.id "
        sql += "WHERE s.id = " + str(save_id) + ") lt "
        sql += "WHERE m.room_theme = lt.room_theme AND m.start_floor <= lt.level AND m.end_floor >= lt.level AND m.is_boss "
        sql += "ORDER BY Rand() "
        sql += "LIMIT 1" 

        res = crsr.execute(sql) #execute the query
        for row in crsr:
            row_string = ''
            for att in row:
                row_string += str(att) + ';'
            query.append(row_string)

    for line in query:
        print(line)

    crsr.close()
    return query

def get_weapon():
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT id, name, type, atk, atk_var, `range`, acc, acc_decay "
    sql += "FROM weapon "
    sql += "ORDER BY Rand() "
    sql += "LIMIT 1"

    res = crsr.execute(sql) #execute the query
    for row in crsr:
        row_string = ''
        for att in row:
            row_string += str(att) + ';'
        query.append(row_string)

    for line in query:
        print(line)

    crsr.close()
    return query

def get_enchant(save_id):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT e.id, e.name, e.atk_mod, e.atk_var_mod, e.acc_mod, e.acc_decay_mod "
    sql += "FROM enchant e, save s "
    sql += "WHERE s.id = " + str(save_id) + " AND e.s_floor <= s.level AND e.e_floor >= s.level "
    sql += "ORDER BY Rand() "
    sql += "LIMIT 1"

    res = crsr.execute(sql) #execute the query
    for row in crsr:
        row_string = ''
        for att in row:
            row_string += str(att) + ';'
        query.append(row_string)

    for line in query:
        print(line)

    crsr.close()
    return query

def get_consumable(save_id):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT * "
    sql += "FROM consumable "
    sql += "ORDER BY Rand() "
    sql += "LIMIT 1"

    res = crsr.execute(sql) #execute the query

    consumable_id = 0
    for row in crsr:
        consumable_id = row[0]
        row_string = ''
        for att in range(7):
            if(att != 0):
                row_string += str(row[att]) + ';'
        query.append(row_string)
    
    inventory_id = add_consumable_to_inventory(save_id, consumable_id)
    if inventory_id != -1:
        query[0] += str(inventory_id) + ";"
        for line in query:
            print(line)
    
    crsr.close()
    return query

def add_consumable_to_inventory(save_id, consumable_id):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT count(*) "
    sql += "FROM inventory "
    sql += "WHERE save = " + str(save_id) + " "
    sql += "GROUP BY save"

    res = crsr.execute(sql) #execute the query
    in_inventory = 0
    for row in crsr:
        in_inventory = row[0]
    
    inventory_id = -1
    if in_inventory < 9:
        sql = "INSERT INTO inventory(save, item) VALUES(" + str(save_id) + "," + str(consumable_id) + ");"
        res = crsr.execute(sql) #execute the query
        db.commit()

        sql = "SELECT LAST_INSERT_ID() "
        sql += "FROM inventory"
        res = crsr.execute(sql) #execute the query

        for row in crsr:
            inventory_id = row[0]

    crsr.close()
    return inventory_id

# def remove_consumable_from_inventory(save_id, consumable_id)

def get_reward(save_id, reward_type):
    print(reward_type)
    if reward_type == 1:
        get_weapon()
        get_enchant(save_id)
    if reward_type == 2:
        get_consumable(save_id)
    

def select_new_map(save_id):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT m.id, m.num_monsters, m.max_mons_types, m.boss_fight, reward_type, m.length, m.width, room_theme "
    sql += "FROM map m, save s "
    sql += "WHERE m.start_floor <= s.level and m.end_floor >= s.level and s.id = " + str(save_id) + " "
    sql += "ORDER BY Rand() "
    sql += "LIMIT 1"

    res = crsr.execute(sql) #execute the query

    for row in crsr:
        id = row[0]
        num_mons = row[1]
        max_mons_types = row[2]
        boss_fight = row[3]
        reward_type = row[4]

        row_string = ''
        for att in range(8):
            if att > 4:
                row_string += str(row[att]) + ';'
        query.append(row_string)

    sql = "UPDATE save "
    sql += "SET on_map = " + str(id) + " "
    sql += "WHERE id = " + str(save_id)

    res = crsr.execute(sql) #execute update query
    db.commit()

    for line in query:
        print(line)

    crsr.close()

    get_list_monsters(save_id, num_mons, max_mons_types, boss_fight)
    get_reward(save_id, reward_type)


def read_file_write_db(save_id):
    return

def read_db_write_file(save_id):
    return

if __name__ == '__main__':
    # if int(sys.argv[1]) == 1:
    #     open("test_passed.txt", "x")
    # get_possible_maps(1)
    select_new_map(1)
    # if(int(sys.argv[1])):
    #     read_file_write_db(int(sys.argv[2]))
    # else:
    #     read_db_write_file(int(sys.argv[2]))
    # get_list_monsters(1, 2)

