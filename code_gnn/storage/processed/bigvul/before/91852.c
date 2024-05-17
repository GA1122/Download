int open_net_fd(const char *spec)
{
    char *p;
    int fd, port;
    struct sockaddr_in sa;
    struct hostent *he;
    p = strchr(spec, ':');
    if (!p)
        return -1;
    *p++ = '\0';
    port = strtol(p, NULL, 10);
     
    sa.sin_family = PF_INET;
    sa.sin_port = htons(port);
    he = gethostbyname(spec);
    if (!he) {
#ifdef HAVE_HERROR
        herror("open_net_fd");
#endif
        return -1;
    }
    memcpy(&sa.sin_addr, he->h_addr, he->h_length);
     
    fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (fd < 0)
        return fd;
    if (connect(fd, (struct sockaddr *) &sa, sizeof (sa)) < 0)
        return -1;
    return fd;
}
