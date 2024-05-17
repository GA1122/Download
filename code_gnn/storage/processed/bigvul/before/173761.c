int main(int argc, char **argv) {
 const char *source_path = NULL;
 const char *label = NULL;
 uid_t uid = 0;
 gid_t gid = 0;
 userid_t userid = 0;
 bool multi_user = false;
 bool full_write = false;
 int i;
 struct rlimit rlim;
 int fs_version;

 int opt;
 while ((opt = getopt(argc, argv, "u:g:U:mw")) != -1) {
 switch (opt) {
 case 'u':
                uid = strtoul(optarg, NULL, 10);
 break;
 case 'g':
                gid = strtoul(optarg, NULL, 10);
 break;
 case 'U':
                userid = strtoul(optarg, NULL, 10);
 break;
 case 'm':
                multi_user = true;
 break;
 case 'w':
                full_write = true;
 break;
 case '?':
 default:
 return usage();
 }
 }

 for (i = optind; i < argc; i++) {
 char* arg = argv[i];
 if (!source_path) {
            source_path = arg;
 } else if (!label) {
            label = arg;
 } else {
            ERROR("too many arguments\n");
 return usage();
 }
 }

 if (!source_path) {
        ERROR("no source path specified\n");
 return usage();
 }
 if (!label) {
        ERROR("no label specified\n");
 return usage();
 }
 if (!uid || !gid) {
        ERROR("uid and gid must be nonzero\n");
 return usage();
 }

    rlim.rlim_cur = 8192;
    rlim.rlim_max = 8192;
 if (setrlimit(RLIMIT_NOFILE, &rlim)) {
        ERROR("Error setting RLIMIT_NOFILE, errno = %d\n", errno);
 }

 while ((fs_read_atomic_int("/data/.layout_version", &fs_version) == -1) || (fs_version < 3)) {
        ERROR("installd fs upgrade not yet complete. Waiting...\n");
        sleep(1);
 }

    run(source_path, label, uid, gid, userid, multi_user, full_write);
 return 1;
}
