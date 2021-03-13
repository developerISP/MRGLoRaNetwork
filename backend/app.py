#!/usr/bin/env python3

import mimetypes as mt
from db_handler.setup import SetupManager as sm
from flask import Flask, render_template
from flask.helpers import make_response


setup = sm(path="db_handler/db.conf")
db = setup.setupDatabase()
db.getRows("stations")
app = Flask(__name__)#, template_folder="backend")

@app.route("/")
def hello():
    with open("index.html") as f:
       data = '\n'.join(f.readlines()) 
    return data

@app.route("/<path:path>")
def get_file(path):
    with open(f"./{path}", "rb") as f:
        data = b''.join(f.readlines())

    r = make_response(data)
    print(path, mt.guess_type(path))
    if (t := mt.guess_type(path)[0]):
        r.mimetype = t
    return r

@app.route("/stations")
def get_stations():
    raw = db.getRows("stations")
    return render_template(
        "stations.html", 
        title = "Stations", 
        data=raw, 
        headers = raw[0].keys()
    )
    #return {n + 1: v for n, v in enumerate(raw)}

@app.route("/sensors_data")
def get_sensors_data():
    raw = db.getRows("sensors_data")
    return {n + 1: v for n, v in enumerate(raw)}

@app.route("/stations_data")
def get_stations_data():
    raw = db.getRows("stations_data")
    return {n + 1: v for n, v in enumerate(raw)}

app.run()