import sys
import random
import pymysql


# load save queries /////////////////////////////////////////////////////////////////////////
#   load save queries
#called argv[1] == 5 and argv[2] = <save_id>
def load_save_file(save_id):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()

    #query to retrieve stats from save table
    sql = "SELECT wep_id, enchant_id, id, max_hp, cur_hp, str, spd "
    sql += "FROM save "
    sql += "WHERE id = " + str(save_id)

    crsr.execute(sql) #execute query

    row = crsr.fetchone()

    #store weapon and enchant id for later reference
    wep_id = row[0]
    enchant_id = row[1]

    #append each attribute of save table tuple except wep_id and enchant_id to list of query results
    row_string = ''
    for att in range(7):
        if att > 1:
            row_string += str(row[att]) + ';'
    query.append(row_string)

    #query to retrieve weapon stats (based on wep_id retrieved earlier)
    sql = "SELECT name, type, atk, atk_var, `range`, acc, acc_decay "
    sql += "FROM weapon " 
    sql += "WHERE id = " + str(wep_id)

    crsr.execute(sql) #execute query

    row = crsr.fetchone()

    #append each attribute of query result to list of query results
    row_string = ''
    for att in row:
        row_string += str(att) + ';'
    query.append(row_string)

    #query to retrieve enchant stats (based on enchant_id retrieved earlier)
    sql = "SELECT name, atk_mod, atk_var_mod, acc_mod, acc_decay_mod "
    sql += "FROM enchant "
    sql += "WHERE id = " + str(enchant_id)

    crsr.execute(sql) #execute the query

    row = crsr.fetchone()

    #append each attribute of query result ot list of query results
    row_string = ''
    for att in row:
        row_string += str(att) + ';'
    query.append(row_string)

    #query to retrieve number of items in inventory
    sql = "SELECT count(*) "
    sql += "FROM inventory "
    sql += "GROUP BY save "
    sql += "HAVING save = " + str(save_id)

    crsr.execute(sql) #execute the query

    row = crsr.fetchone() #fetch the one row that is returned
    if row is not None:
        query.append(str(row[0]) + ";") #append the single attribute of this row to query results

        #query to retrieve all items in inventory
        sql = "SELECT c.id, c.name, c.desc, c.heal, c.spd, c.str, c.dur "
        sql += "FROM inventory i JOIN consumable c ON i.item = c.id "
        sql += "WHERE i.save = " + str(save_id)

        crsr.execute(sql) #execute the query

        #append each attribute of query result ot list of query results
        for row in crsr:
            row_string = ''
            for att in row:
                row_string += str(att) + ';'
            query.append(row_string)
    else:
        query.append("0;") #no items :)

    #write to file
    file = open("data\\runtime_data.txt", "w")

    for line in query:
        file.write(line + "\n")
    
    #clean up
    file.close()
    crsr.close()

#called with argv[1] == 1
def get_save_files():
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT count(*) "
    sql += "FROM save"

    crsr.execute(sql)
    row = crsr.fetchone()
    query.append(str(row[0]) + ";")

    sql = "SELECT * "
    sql += "FROM save"

    crsr.execute(sql) #execute query
    for row in crsr:
        row_string = ''
        for att in row:
            row_string += str(att) + ';'
        query.append(row_string)

    #write to file
    file = open("data\\runtime_data.txt", "w")

    for line in query:
        file.write(line + "\n")
    
    #clean up
    file.close()
    crsr.close()

    #start new save queries
#called with argv[1] == 2
def get_character_list():
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT count(*) "
    sql += "FROM `character` "
    sql += "GROUP BY is_unlocked "
    sql += "HAVING is_unlocked = 1"

    crsr.execute(sql) #execute query

    first_row = crsr.fetchone()
    query.append(str(first_row[0]) + ";")

    sql = "SELECT c.id, c.name, c.hp, c.str, c.spd, w.name "
    sql += "FROM `character` c JOIN weapon w ON c.weapon_id = w.id "
    sql += "WHERE c.is_unlocked"

    crsr.execute(sql) #execute query

    for row in crsr:
        row_string = ''
        for att in row:
            row_string += str(att) + ';'
        query.append(row_string)
    
    #write to file
    file = open("data\\runtime_data.txt", "w")

    for line in query:
        file.write(line + "\n")
    
    #clean up
    file.close()
    crsr.close()

def get_character_stats(character_id):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT name, hp, str, spd, weapon_id "
    sql += "FROM `character` "
    sql += "WHERE id = " + str(character_id)

    crsr.execute(sql) #execute query

    row = crsr.fetchone()
    
    crsr.close() #clean up
    return row

#called with argv[1] == 3 and argv[2] == <character_id>
def start_new_save(character_id):
    character_stats = get_character_stats(character_id)
    name = character_stats[0]
    hp = character_stats[1]
    strength = character_stats[2]
    speed = character_stats[3]
    weapon = character_stats[4]
    
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "INSERT INTO save(name, max_hp, cur_hp, str, spd, wep_id, enchant_id, level, on_map) "
    sql += "VALUES('" + str(name) + "'," + str(hp) + "," + str(hp) + "," + str(strength) + "," + str(speed) + "," + str(weapon) + ",1,1,501)"
    
    crsr.execute(sql)
    db.commit() #confirm table manipulation

    crsr.close() #clean up

#delete save file queries////////////////////////////////////////////////////////////////////
#called with argv[1] == 4 and argv[2] == <save_id>
def delete_save_file(save_id):
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()

    #query to delete tuple with id = "save_id"
    sql = "DELETE FROM save WHERE id = " + str(save_id)
    crsr.execute(sql) #execute query
    db.commit() #confirm table manipulation
    crsr.close() #clean up

#reward queries /////////////////////////////////////////////////////////////////////////////
def get_next_unlock_id(save_id):
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT level "
    sql += "FROM save "
    sql += "WHERE id = " + str(save_id)

    crsr.execute(sql) #execute the query
    row = crsr.fetchone()
    unlock_level = row[0]

    sql = "SELECT id "
    sql += "FROM `character` "
    sql += "WHERE unlock_floor = " + str(unlock_level)

    crsr.execute(sql) #execute the query 
    row = crsr.fetchone()

    crsr.close() #clean up
    return row[0]

def unlock_character(save_id):
    id = get_next_unlock_id(save_id)
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "UPDATE `character` SET is_unlocked = 1 WHERE (id = " + str(id) + ")"
    crsr.execute(sql)
    db.commit() #confirm table manipulation

    crsr.close() #clean up

def get_new_weapon():
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT id, name, type, atk, atk_var, `range`, acc, acc_decay "
    sql += "FROM weapon "
    sql += "ORDER BY Rand() "
    sql += "LIMIT 1"

    crsr.execute(sql) #execute the query
    row = crsr.fetchone()
    row_string = ''
    for att in row:
        row_string += str(att) + ';'
    query.append(row_string)

    #write to file
    file = open("data\\runtime_data.txt", "w")

    for line in query:
        file.write(line + "\n")

    #clean up
    file.close()
    crsr.close()
    return query

def get_new_enchant(save_id):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT e.id, e.name, e.atk_mod, e.atk_var_mod, e.acc_mod, e.acc_decay_mod "
    sql += "FROM enchant e, save s "
    sql += "WHERE s.id = " + str(save_id) + " AND e.s_floor <= s.level AND e.e_floor >= s.level "
    sql += "ORDER BY Rand() "
    sql += "LIMIT 1"

    crsr.execute(sql) #execute the query
    row = crsr.fetchone()
    row_string = ''
    for att in row:
        row_string += str(att) + ';'
    query.append(row_string)

    #write to file
    file = open("data\\runtime_data.txt", "a")

    for line in query:
        file.write(line + "\n")

    #clean up
    crsr.close()
    file.close()
    return query

def swap_weapon(save_id, new_weapon_id, new_enchant_id):
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "UPDATE save SET wep_id = " + str(new_weapon_id) + ", enchant_id = " + str(new_enchant_id) + " WHERE (id = " + str(save_id) + ")" 
    crsr.execute(sql) #execute the query
    db.commit() #confirm table manipulation

    crsr.close() #clean up

def attempt_add_consumable_to_inventory(save_id, consumable_id):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT count(*) "
    sql += "FROM inventory "
    sql += "WHERE save = " + str(save_id) + " "
    sql += "GROUP BY save"

    crsr.execute(sql) #execute the query
    in_inventory = 0
    for row in crsr:
        in_inventory = row[0]
    
    if in_inventory < 9:
        sql = "INSERT INTO inventory(save, item) VALUES(" + str(save_id) + "," + str(consumable_id) + ");"
        crsr.execute(sql) #execute the query
        db.commit() #confirm table manipulation

    crsr.close() #clean up

def get_new_consumable(save_id):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT * "
    sql += "FROM consumable "
    sql += "ORDER BY Rand() "
    sql += "LIMIT 1"

    crsr.execute(sql) #execute the query

    consumable_id = 0
    row = crsr.fetchone()
    consumable_id = row[0]
    row_string = ''
    for att in row:
        row_string += str(att) + ';'
    query.append(row_string)
    
    attempt_add_consumable_to_inventory(save_id, consumable_id)
    
    #write to file
    file = open("data\\runtime_data.txt", "w")

    for line in query:
        file.write(line + "\n")
    
    #clean up
    file.close()
    crsr.close()
    return query

#called with argv[1] = 7 argv[2] = <save_file> argv[3] = <reward_type>
def get_reward(save_id, reward_type):
    if reward_type == 1:
        get_new_weapon()
        get_new_enchant(save_id)
    elif reward_type == 2:
        get_new_consumable(save_id)
    elif reward_type == 4:
        unlock_character(save_id)

#called with 
def swap_consumables(save_id, deletion_consumable_id, new_consumable_id):
    id = get_inventory_id(save_id, deletion_consumable_id)
    if id != -1:
        #connect to mysql dbms
        db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
        crsr = db.cursor()
        sql = "UPDATE inventory SET item = " + str(new_consumable_id) + " WHERE (id = " + str(id) + ")" 
        crsr.execute(sql) #execute the query
        db.commit() #confirm table manipulation

        crsr.close()#clean up

#update map queries/////////////////////////////////////////////////////////////////////////
def get_list_monsters(save_id, num_monsters, max_mons_types, boss_fight):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT m.name, m.description, m.sight_range, m.atk_strength, m.atk_var, m.atk_range, m.move, m.acc, m.acc_decay, m.symbol, m.max_hp, m.default_beh, m.blind_beh, m.provoked_beh "
    sql += "FROM monster m, "
    sql += "(SELECT s.level, ma.room_theme "
    sql += "FROM save s join map ma ON s.on_map = ma.id "
    sql += "WHERE s.id = " + str(save_id) + ") lt "
    sql += "WHERE m.room_theme = lt.room_theme AND m.start_floor <= lt.level AND m.end_floor >= lt.level AND NOT m.is_boss "
    sql += "ORDER BY Rand() "
    sql += "LIMIT " + str(max_mons_types)

    crsr.execute(sql) #execute the query
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

        crsr.execute(sql) #execute the query
        row = crsr.fetchone()
        row_string = ''
        for att in row:
            row_string += str(att) + ';'
        query.append(row_string)

    #write to file
    file = open("data\\runtime_data.txt", "a")

    for line in query:
        file.write(line + "\n")

    #clean up
    file.close()
    crsr.close()
    return query

#called with argv[1] = 6, argv[2] = <save_id>, and argv[3] = <just_loaded>
def select_map(save_id, just_loaded):
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    if not just_loaded:
        sql = "SELECT m.id, m.max_mons_types, m.boss_fight, reward_type, m.length, m.width, room_theme, m.num_monsters "
        sql += "FROM map m, save s "
        sql += "WHERE m.start_floor <= s.level and m.end_floor >= s.level and s.id = " + str(save_id) + " "
        sql += "ORDER BY Rand() "
        sql += "LIMIT 1"
    else:
        sql = "SELECT m.id, m.max_mons_types, m.boss_fight, reward_type, m.length, m.width, room_theme, m.num_monsters "
        sql += "FROM map m, save s "
        sql += "WHERE s.on_map = m.id AND s.id = " + str(save_id)

    crsr.execute(sql) #execute the query

    
    row = crsr.fetchone()
    id = row[0]
    num_mons = row[7]
    mons_plus_boss = num_mons
    max_mons_types = row[1]
    boss_fight = row[2]

    if(boss_fight):
        mons_plus_boss += 1

    row_string = ''
    for att in range(7):
        if att > 2:
            row_string += str(row[att]) + ';'
    row_string += str(mons_plus_boss) + ';'
    query.append(row_string)


    sql = "UPDATE save "
    sql += "SET on_map = " + str(id) + " "
    sql += "WHERE id = " + str(save_id)

    crsr.execute(sql) #execute update query
    db.commit() #confirm table manipulation

    #write to file
    file = open("data\\runtime_data.txt", "w")

    for line in query:
        file.write(line + "\n")

    #clean up
    file.close()
    crsr.close()

    get_list_monsters(save_id, num_mons, max_mons_types, boss_fight)

#update save query///////////////////////////////////////////////////////////////////////////
#called with argv[1] = 8, argv[2] = <save_id>, and argv[3] = update 
#update is string like so: "<max_hp>,<cur_hp>,<str>,<spd>"
def update_save(save_id, update):
    update_stat_list = update.split(',')
    max_hp = update_stat_list[0]
    cur_hp = update_stat_list[1]
    strength = update_stat_list[2]
    spd = update_stat_list[3]

    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()

    sql = "SELECT level "
    sql += "FROM save "
    sql += "WHERE id = " + str(save_id)

    crsr.execute(sql) #execute query

    row = crsr.fetchone()

    level = int(row[0]) + 1

    sql = "UPDATE save SET max_hp = " + str(max_hp) + ", cur_hp = " + str(cur_hp) + ", str = " + str(strength)
    sql += ", spd = " + str(spd) + ", level = " + str(level) + " WHERE id = " + str(save_id)

    crsr.execute(sql)
    db.commit() #confirm table manipulation

    crsr.close() #clean up

#use consumables query///////////////////////////////////////////////////////////////////////
def get_inventory_id(save_id, consumable_id):
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT id "
    sql += "FROM inventory "
    sql += "WHERE save = " + str(save_id) + " AND item = " + str(consumable_id) + " "
    sql += "LIMIT 1"

    crsr.execute(sql) #execute the query
    id = -1
    row = crsr.fetchone()
    if row is not None:
        id = row[0]
    
    crsr.close() #clean up
    return id

#called with argv[1] = 10, argv[2] = <save_id>, and argv[3] = <consumable_id>
def use_consumable(save_id, consumable_id):
    id = get_inventory_id(save_id, consumable_id)
    if id != -1:
        #connect to mysql dbms
        db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
        crsr = db.cursor()
        sql = "DELETE FROM inventory WHERE (id = " + str(id) + ")"
        crsr.execute(sql) #execute the query
        db.commit() #confirm table manipulation

        crsr.close() #clean up

def choose_query():
    if int(sys.argv[1]) == 1:
        get_save_files()
    elif int(sys.argv[1]) == 2:
        get_character_list()
    elif int(sys.argv[1]) == 3:
        start_new_save(int(sys.argv[2]))
    elif int(sys.argv[1]) == 4:
        delete_save_file(int(sys.argv[2]))
    elif int(sys.argv[1]) == 5:
        load_save_file(int(sys.argv[2]))
    elif int(sys.argv[1]) == 6:
        select_map(int(sys.argv[2]), bool(int(sys.argv[3])))
    elif int(sys.argv[1]) == 7:
        get_reward(int(sys.argv[2]), int(sys.argv[3]))
    elif int(sys.argv[1]) == 8:
        update_save(int(sys.argv[2]), str(sys.argv[3]))
    elif int(sys.argv[1]) == 9:
        swap_consumables(int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4]))
    elif int(sys.argv[1]) == 10:
        use_consumable(int(sys.argv[2]), int(sys.argv[3]))
    elif int(sys.argv[1]) == 11:
        swap_weapon(int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4]))

if __name__ == '__main__':
    choose_query()
    # select_map(15, False)