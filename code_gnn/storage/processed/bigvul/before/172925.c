static void lsof_dumpinfo(pid_t pid)
{
 int fd;
 struct pid_info_t info;
 struct stat pidstat;
 struct passwd *pw;

    info.pid = pid;
    snprintf(info.path, sizeof(info.path), "/proc/%d/", pid);
    info.parent_length = strlen(info.path);

 if (!stat(info.path, &pidstat)) {
        pw = getpwuid(pidstat.st_uid);
 if (pw) {
            strlcpy(info.user, pw->pw_name, sizeof(info.user));
 } else {
            snprintf(info.user, USER_DISPLAY_MAX, "%d", (int)pidstat.st_uid);
 }
 } else {
        strcpy(info.user, "???");
 }

    strlcat(info.path, "cmdline", sizeof(info.path));
    fd = open(info.path, O_RDONLY);
 if (fd < 0) {
        fprintf(stderr, "Couldn't read %s\n", info.path);
 return;
 }

 char cmdline[PATH_MAX];
 int numRead = read(fd, cmdline, sizeof(cmdline) - 1);
    close(fd);

 if (numRead < 0) {
        fprintf(stderr, "Error reading cmdline: %s: %s\n", info.path, strerror(errno));
 return;
 }

    cmdline[numRead] = '\0';

    strlcpy(info.cmdline, basename(cmdline), sizeof(info.cmdline));

    print_type("cwd", &info);
    print_type("exe", &info);
    print_type("root", &info);

    print_fds(&info);
    print_maps(&info);
}
