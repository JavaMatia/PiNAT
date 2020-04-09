from Plugin_Observer import plugin
from bin import pynat
import sqlite3
import os
import json

class IPBlocker(plugin):

    def __init__(self):
        self.name = "IPBlocker"
        self.type = "blocker"
        self.version = 1.0
        self.description = "Blocks certain external ip addresses"
        self.author = "Ofri Marx"
        self.priority = 1000
        self.actions = ["get_blocked_ips", "get_blocked_stats"]
        
        self.blacklist = []
        self.db = ""


    def process(self, packet):
        src_addr = ""
        dst_addr = ""
        ips = pynat.get_ips(packet)
        if ips == None: return packet
        src_addr, dst_addr = ips[0], ips[1]
        if dst_addr in self.blacklist:
            pynat.exec_db(self.db, "INSERT OR IGNORE INTO LOG VALUES ('{}', '{}', strftime('%Y-%m-%d %H:%M', 'now', 'localtime'))".format(src_addr, dst_addr))
            pynat.drop_packet(packet)
            return None

        return packet


    def setup(self):
        file_location = os.path.dirname(__file__)
        self.db = pynat.open_db(file_location + "/{}.db".format(self.name))
        pynat.exec_db(self.db, "CREATE TABLE IF NOT EXISTS LOG (SRC_IP TEXT NOT NULL, BLOCKED_IP TEXT NOT NULL, TIME TEXT NOT NULL, UNIQUE(SRC_IP, BLOCKED_IP, TIME))")
        pynat.exec_db(self.db, "CREATE TABLE IF NOT EXISTS BLACKLIST (BLOCKED_IP TEXT NOT NULL)")

        self.blacklist = [row[0] for row in pynat.select_db(self.db, "SELECT BLOCKED_IP FROM BLACKLIST")]
        


    def teardown(self):
        pynat.close_db(self.db)

    
    # return a list of available actions!
    def get_actions(self):
        return self.actions


    def delete_database(self):
        pynat.exec_db(self.db, "DELETE FROM LOG")


    def get_blocked_ips(self):
        return {"result": self.blacklist}


    def get_blocked_stats(self):
        answer_array = []
        db_res = pynat.exec_db(self.db, "SELECT * FROM LOG")
        
        for entry in db_res:
            answer_array.append({"src": entry[0], "dst": entry[1], "time": entry[2]})

        return {"result": answer_array}
