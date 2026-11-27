TEMPLATE = subdirs

SUBDIRS = \
    common \
    server \
    client \
    profiles

server.depends = common
client.depends = common