static int read_package_list(struct fuse_global* global) {
    pthread_mutex_lock(&global->lock);

    hashmapForEach(global->package_to_appid, remove_str_to_int, global->package_to_appid);

 FILE* file = fopen(kPackagesListFile, "r");
 if (!file) {
        ERROR("failed to open package list: %s\n", strerror(errno));
        pthread_mutex_unlock(&global->lock);
 return -1;
 }

 char buf[512];
 while (fgets(buf, sizeof(buf), file) != NULL) {
 char package_name[512];
 int appid;
 char gids[512];

 if (sscanf(buf, "%s %d %*d %*s %*s %s", package_name, &appid, gids) == 3) {
 char* package_name_dup = strdup(package_name);
            hashmapPut(global->package_to_appid, package_name_dup, (void*) (uintptr_t) appid);
 }
 }

    TRACE("read_package_list: found %zu packages\n",
            hashmapSize(global->package_to_appid));
    fclose(file);

  
    derive_permissions_recursive_locked(global->fuse_default, &global->root);

    pthread_mutex_unlock(&global->lock);
 return 0;
}
