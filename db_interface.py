import sys
import pymysql
import random

def get_list_monsters(save_id, num_monsters):
    result = []
    query = []
    #connect to mysql dbms
    db = pymysql.connect(host='rpggame.ctskhbc7cwkq.us-east-2.rds.amazonaws.com', user='admin', password='saul22gone', database='rpggame')
    crsr = db.cursor()
    sql = "SELECT m.name, m.description, m.sight_range, m.atk_strength, m.atk_var, m.atk_range, m.move, m.acc, m.acc_decay, m.symbol, m.max_hp, m.default_beh, m.blind_beh, m.provoked_beh, m.is_boss "
    sql += "FROM monster m, "
    sql += "(SELECT s.level, ma.room_theme, ma.max_duplicate_mons "
    sql += "FROM save s join map ma ON s.on_map = ma.id "
    sql += "WHERE s.id = " + str(save_id) + ") lt "
    sql += 'WHERE m.room_theme = lt.room_theme AND m.start_floor <= lt.level AND m.end_floor >= lt.level '
    sql += 'ORDER BY Rand() '
    sql += 'LIMIT ma.max_duplicate_mons'

    res = crsr.execute(sql) #execute the query
    #append a formatted row of the result table to a list which will be attached to result and returned to user
    row_string = ''
    for row in crsr:
        for att in row:
            row_string += str(att) + ','
        query.append(row_string)

    while(len(query) < num_monsters):
        query.append(random.choice(query))

    crsr.close()
    return query

# def get_possible_maps(save_id):


def read_file_write_db(save_id):
    return

def read_db_write_file(save_id):
    return

if __name__ == '__main__':
    if(int(sys.argv[1])):
        read_file_write_db(int(sys.argv[2]))
    else:
        read_db_write_file(int(sys.argv[2]))
    # get_list_monsters(1, 2)

