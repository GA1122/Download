static int vnc_server_info_put(QDict *qdict)
{
    if (vnc_server_addr_put(qdict, vnc_display->lsock) < 0) {
        return -1;
    }

    qdict_put(qdict, "auth", qstring_from_str(vnc_auth_name(vnc_display)));
    return 0;
}
