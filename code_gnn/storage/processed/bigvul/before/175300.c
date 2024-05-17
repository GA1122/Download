void listenCallback_helper(int fd, short flags, void *param) {
    android::listenCallback(fd, flags, param);
}
