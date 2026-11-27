TEMPLATE = subdirs

SUBDIRS = \
    common \
    server \
    client

server.depends = common
client.depends = common