static char* find_file_within(const char* path, const char* name,
 char* buf, size_t bufsize, int search)
{
 size_t pathlen = strlen(path);
 size_t namelen = strlen(name);
 size_t childlen = pathlen + namelen + 1;
 char* actual;

 if (bufsize <= childlen) {
 return NULL;
 }

    memcpy(buf, path, pathlen);
    buf[pathlen] = '/';
    actual = buf + pathlen + 1;
    memcpy(actual, name, namelen + 1);

 if (search && access(buf, F_OK)) {
 struct dirent* entry;
 DIR* dir = opendir(path);
 if (!dir) {
            ERROR("opendir %s failed: %s\n", path, strerror(errno));
 return actual;
 }
 while ((entry = readdir(dir))) {
 if (!strcasecmp(entry->d_name, name)) {
  
                memcpy(actual, entry->d_name, namelen);
 break;
 }
 }
        closedir(dir);
 }
 return actual;
}
