bind_and_listen(struct addrinfo *addr)
{
    int optval;
    int fd;
    int rc;
    char buffer[256] = { 0, };

    if (addr->ai_family == AF_INET6) {
        struct sockaddr_in6 *addr_in = (struct sockaddr_in6 *)(void*)addr->ai_addr;
        inet_ntop(addr->ai_family, &addr_in->sin6_addr, buffer, DIMOF(buffer));

    } else {
        struct sockaddr_in *addr_in = (struct sockaddr_in *)(void*)addr->ai_addr;
        inet_ntop(addr->ai_family, &addr_in->sin_addr, buffer, DIMOF(buffer));
    }

    crm_trace("Attempting to bind on address %s", buffer);

    fd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (fd < 0) {
        return -1;
    }

     
    optval = 1;
    rc = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (rc < 0) {
        crm_perror(LOG_INFO, "Couldn't allow the reuse of local addresses by our remote listener, bind address %s", buffer);
        close(fd);
        return -1;
    }

    if (addr->ai_family == AF_INET6) {
        optval = 0;
        rc = setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &optval, sizeof(optval));
        if (rc < 0) {
            crm_perror(LOG_INFO, "Couldn't disable IPV6 only on address %s", buffer);
            close(fd);
            return -1;
        }
    }

    if (bind(fd, addr->ai_addr, addr->ai_addrlen) != 0) {
        close(fd);
        return -1;
    }

    if (listen(fd, 10) == -1) {
        crm_err("Can not start listen on address %s", buffer);
        close(fd);
        return -1;
    }

    crm_notice("Listening on address %s", buffer);

    return fd;
}