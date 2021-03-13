from types import LambdaType
from typing import Dict, List, Sequence, Union
import sqlalchemy as sa
from sqlalchemy import insert
from sqlalchemy.orm import Session
from .schemas import *

class DatabaseManager():

    def __init__(self, username, password, host):
        self.__engine = sa.create_engine(f"postgresql://{username}:{password}@{host}/cnr_isp data")
        
        with self.__engine.connect() as conn:
            if conn != None:
                print("[+] Database connected")
            else:
                print("[-] Database connection failed")

    def addRow(self, table_name: str, row_data: Union[Sequence, dict]):
        if type(row_data) == dict:
            t = get_schema(table_name)
            if t != None:
                conn = self.__engine.connect()
                with self.__engine.begin() as conn:
                    _ = conn.execute(sa.insert(t).values(row_data))

    def getRows(self, table_name, clauses: Dict[str,LambdaType] = None) -> List:
        t = get_schema(table_name)
        if t != None:
            res = []
            with self.__engine.begin() as conn:
                for r in conn.execute(sa.select(t.c)):
                    res.append({k: v for k, v in r.items()})

            return res
                    
    def create_table(self, schema: sa.Table):
        meta = schema.metadata
        meta.create_all(self.__engine, tables=[schema], checkfirst=True)
