static int vnc_server_addr_put(QDict *qdict, int fd)
{
    struct sockaddr_storage sa;
    socklen_t salen;

    salen = sizeof(sa);
    if (getsockname(fd, (struct sockaddr*)&sa, &salen) < 0) {
        return -1;
    }

    return put_addr_qdict(qdict, &sa, salen);
}
