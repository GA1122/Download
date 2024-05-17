static int put_addr_qdict(QDict *qdict, struct sockaddr_storage *sa,
                          socklen_t salen)
{
    char host[NI_MAXHOST];
    char serv[NI_MAXSERV];
    int err;

    if ((err = getnameinfo((struct sockaddr *)sa, salen,
                           host, sizeof(host),
                           serv, sizeof(serv),
                           NI_NUMERICHOST | NI_NUMERICSERV)) != 0) {
        VNC_DEBUG("Cannot resolve address %d: %s\n",
                  err, gai_strerror(err));
        return -1;
    }

    qdict_put(qdict, "host", qstring_from_str(host));
    qdict_put(qdict, "service", qstring_from_str(serv));
    qdict_put(qdict, "family",qstring_from_str(inet_strfamily(sa->ss_family)));

    return 0;
}
