static void run(const char* source_path, const char* label, uid_t uid,
 gid_t gid, userid_t userid, bool multi_user, bool full_write) {
 struct fuse_global global;
 struct fuse fuse_default;
 struct fuse fuse_read;
 struct fuse fuse_write;
 struct fuse_handler handler_default;
 struct fuse_handler handler_read;
 struct fuse_handler handler_write;
 pthread_t thread_default;
 pthread_t thread_read;
 pthread_t thread_write;

    memset(&global, 0, sizeof(global));
    memset(&fuse_default, 0, sizeof(fuse_default));
    memset(&fuse_read, 0, sizeof(fuse_read));
    memset(&fuse_write, 0, sizeof(fuse_write));
    memset(&handler_default, 0, sizeof(handler_default));
    memset(&handler_read, 0, sizeof(handler_read));
    memset(&handler_write, 0, sizeof(handler_write));

    pthread_mutex_init(&global.lock, NULL);
    global.package_to_appid = hashmapCreate(256, str_hash, str_icase_equals);
    global.uid = uid;
    global.gid = gid;
    global.multi_user = multi_user;
    global.next_generation = 0;
    global.inode_ctr = 1;

    memset(&global.root, 0, sizeof(global.root));
    global.root.nid = FUSE_ROOT_ID;  
    global.root.refcount = 2;
    global.root.namelen = strlen(source_path);
    global.root.name = strdup(source_path);
    global.root.userid = userid;
    global.root.uid = AID_ROOT;
    global.root.under_android = false;

    strcpy(global.source_path, source_path);

 if (multi_user) {
        global.root.perm = PERM_PRE_ROOT;
        snprintf(global.obb_path, sizeof(global.obb_path), "%s/obb", source_path);
 } else {
        global.root.perm = PERM_ROOT;
        snprintf(global.obb_path, sizeof(global.obb_path), "%s/Android/obb", source_path);
 }

    fuse_default.global = &global;
    fuse_read.global = &global;
    fuse_write.global = &global;

    global.fuse_default = &fuse_default;
    global.fuse_read = &fuse_read;
    global.fuse_write = &fuse_write;

    snprintf(fuse_default.dest_path, PATH_MAX, "/mnt/runtime/default/%s", label);
    snprintf(fuse_read.dest_path, PATH_MAX, "/mnt/runtime/read/%s", label);
    snprintf(fuse_write.dest_path, PATH_MAX, "/mnt/runtime/write/%s", label);

    handler_default.fuse = &fuse_default;
    handler_read.fuse = &fuse_read;
    handler_write.fuse = &fuse_write;

    handler_default.token = 0;
    handler_read.token = 1;
    handler_write.token = 2;

    umask(0);

 if (multi_user) {
  
 if (fuse_setup(&fuse_default, AID_SDCARD_RW, 0006)
 || fuse_setup(&fuse_read, AID_EVERYBODY, 0027)
 || fuse_setup(&fuse_write, AID_EVERYBODY, full_write ? 0007 : 0027)) {
            ERROR("failed to fuse_setup\n");
            exit(1);
 }
 } else {
  
 if (fuse_setup(&fuse_default, AID_SDCARD_RW, 0006)
 || fuse_setup(&fuse_read, AID_EVERYBODY, full_write ? 0027 : 0022)
 || fuse_setup(&fuse_write, AID_EVERYBODY, full_write ? 0007 : 0022)) {
            ERROR("failed to fuse_setup\n");
            exit(1);
 }
 }

  
 if (setgroups(sizeof(kGroups) / sizeof(kGroups[0]), kGroups) < 0) {
        ERROR("cannot setgroups: %s\n", strerror(errno));
        exit(1);
 }
 if (setgid(gid) < 0) {
        ERROR("cannot setgid: %s\n", strerror(errno));
        exit(1);
 }
 if (setuid(uid) < 0) {
        ERROR("cannot setuid: %s\n", strerror(errno));
        exit(1);
 }

 if (multi_user) {
        fs_prepare_dir(global.obb_path, 0775, uid, gid);
 }

 if (pthread_create(&thread_default, NULL, start_handler, &handler_default)
 || pthread_create(&thread_read, NULL, start_handler, &handler_read)
 || pthread_create(&thread_write, NULL, start_handler, &handler_write)) {
        ERROR("failed to pthread_create\n");
        exit(1);
 }

    watch_package_list(&global);
    ERROR("terminated prematurely\n");
    exit(1);
}
