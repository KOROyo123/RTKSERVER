TEMPLATE = subdirs

SUBDIRS= src \
         app \
    koro \
         korolib \
         next \
#         learn

app.depends = src
