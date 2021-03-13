from typing import Any, Dict, List, Union
from sqlalchemy import Table, Column, MetaData
from dataclasses import Field, is_dataclass

# TODO: add conversion from Python types to SQLAlchemy types
# Attributes for internal use only
__tables = {}
__meta = MetaData()
__to_reference = []

# Generates a table schema from a dataclass
def create_schema_from_dataclass(
    model: Any, 
    table_name: str = None,         # custom table name (default: class name)
    primary_key: str = None,        # PK column (default: first field)
    sizes: Dict = None,             # fixed sizes for fields -> {field: size}
    opts: Dict[str, Union[List, str]] = None # autoincrement, not null etc.
) -> Table:

    if not is_dataclass(model):
        raise TypeError(f"{model.__name__} is not a dataclass")

    if table_name == None: 
        table_name = model.__name__
    
    cols, references =  __generate_columns(model.__attribute_fields__)
    __tables[table_name] = Table (
        table_name, __meta,
        *cols
    )

    __to_reference.extend(references)

def __generate_columns(
    model: Dict[str, Field], 
    primary_key: str = None, 
    sizes: Dict = {},            
    opts: Dict[str, Union[List, str]] = {}
) -> List[Column]:
    res = []
    for f in model:
        res.append(Column(f, model[f].type))
        
        

def get_schema(table_name: str) -> Table:
    try:
        return __tables[table_name]
    except KeyError as e:
        return None