static int get_process_info(pid_t tid, pid_t* out_pid, uid_t* out_uid, uid_t* out_gid) {
 char path[64];
  snprintf(path, sizeof(path), "/proc/%d/status", tid);

 FILE* fp = fopen(path, "r");
 if (!fp) {
 return -1;
 }

 int fields = 0;
 char line[1024];
 while (fgets(line, sizeof(line), fp)) {
 size_t len = strlen(line);
 if (len > 6 && !memcmp(line, "Tgid:\t", 6)) {
 *out_pid = atoi(line + 6);
      fields |= 1;
 } else if (len > 5 && !memcmp(line, "Uid:\t", 5)) {
 *out_uid = atoi(line + 5);
      fields |= 2;
 } else if (len > 5 && !memcmp(line, "Gid:\t", 5)) {
 *out_gid = atoi(line + 5);
      fields |= 4;
 }
 }
  fclose(fp);
 return fields == 7 ? 0 : -1;
}
