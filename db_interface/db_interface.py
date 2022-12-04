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

def select_new_map(save_id):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT m.id, m.num_monsters, m.max_mons_types, m.boss_fight, m.length, m.width, reward_type, room_theme "
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

        row_string = ''
        for att in range(7):
            if att != 0 and att != 1 and att != 2 and att != 3:
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

