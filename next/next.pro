TEMPLATE = subdirs


linux{
    SUBDIRS=RTKSERVER_v2\
            rtk_funtest \
            server_mult \
            IMX6U_str2str \

}

win32 {
    SUBDIRS=RTKSERVER_v2\
            rtk_funtest \
            server_mult

}
