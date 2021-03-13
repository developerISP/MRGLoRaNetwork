from .db import DatabaseManager as dm
import configparser as cp
from .schemas import *

class SetupManager():
    
    def __init__(self, path=r"backend/db_handler/db.conf"):
        self.__cfg = cp.ConfigParser()
        self.__cfg.read_file(open(path))
        self.__db = None

    def setupDatabase(self, create_tables: bool =True) -> dm:
        un = self.__cfg.get("User", "username")[1:-1] # removes the first " and the last "
        pw = self.__cfg.get("User", "password")[1:-1]
        host = self.__cfg.get("User", "host")[1:-1]

        if self.__db == None:
            self.__db = dm(un, pw, host)

        if create_tables:
            for s in get_all_schemas():
                self.__db.create_table(s)
        
        return self.__db


    