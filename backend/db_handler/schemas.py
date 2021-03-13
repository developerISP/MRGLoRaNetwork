from sqlalchemy import Table, MetaData, Column, DateTime, Integer, Float, String
from sqlalchemy.orm import relationship
from sqlalchemy.sql.schema import ForeignKey, ForeignKeyConstraint

__meta = MetaData()
__schemas = {}

__schemas["stations"] = Table(
    "stations", __meta,
    Column("addr", String, primary_key=True)
)

__schemas["stations_data"] = Table(
    "stations_data", __meta,
    Column("data_id", Integer, autoincrement=True, primary_key=True),
    Column("time", DateTime),
    Column("addr", String, ForeignKey("stations.addr")),
    Column("reads", Integer),
    Column("tension", Float),
    Column("amps", Float),
    Column("sn_lora", String),
    Column("rssi_lora", String),

)

__schemas["sensors_data"] = Table(
    "sensors_data", __meta,
    Column("read_id", Integer, autoincrement=True, primary_key=True),
    Column("addr", String, ForeignKey("stations.addr")),
    Column("read_number", Integer),
    Column("time", DateTime),
    Column("temp_ext", Float),
    Column("temp_int", Float),
    Column("temp_ext_sd", Float),
    Column("temp_int_sd", Float),
    Column("rh_ext", Float),
    Column("rh_int", Float),
    Column("rh_ext_sd", Float),
    Column("rh_int_sd", Float),
    Column("PM1", Float),
    Column("PM25", Float),
    Column("PM10", Float),
    Column("PM1_sd", Float),
    Column("PM25_sd", Float),
    Column("PM10_sd", Float),
    Column("OZO_11", Float),
    Column("OZO_12", Float),
    Column("OZO_21", Float),
    Column("OZO_22", Float),
    Column("OZO_31", Float),
    Column("OZO_32", Float),
    Column("OZO_11_sd", Float),
    Column("OZO_12_sd", Float),
    Column("OZO_21_sd", Float),
    Column("OZO_22_sd", Float),
    Column("OZO_31_sd", Float),
    Column("OZO_32_sd", Float),
    Column("accX", Float),
    Column("accY", Float),
    Column("accZ", Float),
    Column("accX_sd", Float),
    Column("accY_sd", Float),
    Column("accZ_sd", Float),
    Column("magX", Float),
    Column("magY", Float),
    Column("magZ", Float),
    Column("magX_sd", Float),
    Column("magY_sd", Float),
    Column("magZ_sd", Float),
    Column("gyrX", Float),
    Column("gyrY", Float),
    Column("gyrZ", Float),
    Column("gyrX_sd", Float),
    Column("gyrY_sd", Float),
    Column("gyrZ_sd", Float),
    Column("windD", Float),
    Column("windS", Float),
    Column("windD_sd", Float),
    Column("windS_sd", Float),

    # Relationships
    # relationship(__schemas["stations"])
)

def get_schema(schema_name: str) -> Table:
    return __schemas.get(schema_name)

def get_all_schemas():
    return __schemas.values()