static int connect_namedsocket(const char *path)
{
    int sockfd, size;
    struct sockaddr_un helper;

    if (strlen(path) >= sizeof(helper.sun_path)) {
        error_report("Socket name too long");
        return -1;
    }
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sockfd < 0) {
        error_report("Failed to create socket: %s", strerror(errno));
        return -1;
    }
    strcpy(helper.sun_path, path);
    helper.sun_family = AF_UNIX;
    size = strlen(helper.sun_path) + sizeof(helper.sun_family);
    if (connect(sockfd, (struct sockaddr *)&helper, size) < 0) {
        error_report("Failed to connect to %s: %s", path, strerror(errno));
        close(sockfd);
        return -1;
    }

     
    unlink(path);
    return sockfd;
}
