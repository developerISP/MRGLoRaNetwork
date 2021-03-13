FROM nginx/unit:1.19.0-python3.7

EXPOSE 80

COPY requirements.txt /config/requirements.txt
COPY config.json /docker-entrypoint.d/config.json
COPY app.py /www/app.py

RUN apt update && apt install -y python3-pip    \
    && pip3 install -r /config/requirements.txt \
    && rm -rf /var/lib/apt/lists/*