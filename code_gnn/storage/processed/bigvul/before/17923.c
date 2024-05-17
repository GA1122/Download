static int vnc_qdict_remote_addr(QDict *qdict, int fd)
{
    struct sockaddr_storage sa;
    socklen_t salen;

    salen = sizeof(sa);
    if (getpeername(fd, (struct sockaddr*)&sa, &salen) < 0) {
        return -1;
    }

    return put_addr_qdict(qdict, &sa, salen);
}
