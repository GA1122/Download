static void parseRedisUri(const char *uri) {

    const char *scheme = "redis://";
    const char *curr = uri;
    const char *end = uri + strlen(uri);
    const char *userinfo, *username, *port, *host, *path;

     
    if (strncasecmp(scheme, curr, strlen(scheme))) {
        fprintf(stderr,"Invalid URI scheme\n");
        exit(1);
    }
    curr += strlen(scheme);
    if (curr == end) return;

     
    if ((userinfo = strchr(curr,'@'))) {
        if ((username = strchr(curr, ':')) && username < userinfo) {
             
            curr = username + 1;
        }

        config.auth = percentDecode(curr, userinfo - curr);
        curr = userinfo + 1;
    }
    if (curr == end) return;

     
    path = strchr(curr, '/');
    if (*curr != '/') {
        host = path ? path - 1 : end;
        if ((port = strchr(curr, ':'))) {
            config.hostport = atoi(port + 1);
            host = port - 1;
        }
        config.hostip = sdsnewlen(curr, host - curr + 1);
    }
    curr = path ? path + 1 : end;
    if (curr == end) return;

     
    config.dbnum = atoi(curr);
}
