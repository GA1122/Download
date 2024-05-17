int restartServer(int flags, mstime_t delay) {
    int j;

     
    if (access(server.executable,X_OK) == -1) return C_ERR;

     
    if (flags & RESTART_SERVER_CONFIG_REWRITE &&
        server.configfile &&
        rewriteConfig(server.configfile) == -1) return C_ERR;

     
    if (flags & RESTART_SERVER_GRACEFULLY &&
        prepareForShutdown(SHUTDOWN_NOFLAGS) != C_OK) return C_ERR;

     
    for (j = 3; j < (int)server.maxclients + 1024; j++) close(j);

     
    if (delay) usleep(delay*1000);
    execve(server.executable,server.exec_argv,environ);

     
    _exit(1);

    return C_ERR;  
}
