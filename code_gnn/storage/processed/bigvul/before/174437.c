static void dump_log_file(
 log_t* log, pid_t pid, const char* filename, unsigned int tail) {
 bool first = true;
 struct logger_list* logger_list;

 if (!log->should_retrieve_logcat) {
 return;
 }

  logger_list = android_logger_list_open(
      android_name_to_log_id(filename), O_RDONLY | O_NONBLOCK, tail, pid);

 if (!logger_list) {
    ALOGE("Unable to open %s: %s\n", filename, strerror(errno));
 return;
 }

 struct log_msg log_entry;

 while (true) {
 ssize_t actual = android_logger_list_read(logger_list, &log_entry);
 struct logger_entry* entry;

 if (actual < 0) {
 if (actual == -EINTR) {
 continue;
 } else if (actual == -EAGAIN) {
 break;
 } else {
        _LOG(log, logtype::ERROR, "Error while reading log: %s\n",
          strerror(-actual));
 break;
 }
 } else if (actual == 0) {
      _LOG(log, logtype::ERROR, "Got zero bytes while reading log: %s\n",
        strerror(errno));
 break;
 }


    entry = &log_entry.entry_v1;

 if (first) {
      _LOG(log, logtype::LOGS, "--------- %slog %s\n",
        tail ? "tail end of " : "", filename);
      first = false;
 }

 static const char* kPrioChars = "!.VDIWEFS";
 unsigned hdr_size = log_entry.entry.hdr_size;
 if (!hdr_size) {
      hdr_size = sizeof(log_entry.entry_v1);
 }
 char* msg = reinterpret_cast<char*>(log_entry.buf) + hdr_size;

 char timeBuf[32];
 time_t sec = static_cast<time_t>(entry->sec);
 struct tm tmBuf;
 struct tm* ptm;
    ptm = localtime_r(&sec, &tmBuf);
    strftime(timeBuf, sizeof(timeBuf), "%m-%d %H:%M:%S", ptm);

 if (log_entry.id() == LOG_ID_EVENTS) {
 if (!g_eventTagMap) {
        g_eventTagMap = android_openEventTagMap(EVENT_TAG_MAP_FILE);
 }
 AndroidLogEntry e;
 char buf[512];
      android_log_processBinaryLogBuffer(entry, &e, g_eventTagMap, buf, sizeof(buf));
      _LOG(log, logtype::LOGS, "%s.%03d %5d %5d %c %-8s: %s\n",
         timeBuf, entry->nsec / 1000000, entry->pid, entry->tid,
 'I', e.tag, e.message);
 continue;
 }

 unsigned char prio = msg[0];
 char* tag = msg + 1;
    msg = tag + strlen(tag) + 1;

 char* nl = msg + strlen(msg) - 1;
 while (nl >= msg && *nl == '\n') {
 *nl-- = '\0';
 }

 char prioChar = (prio < strlen(kPrioChars) ? kPrioChars[prio] : '?');

 do {
      nl = strchr(msg, '\n');
 if (nl) {
 *nl = '\0';
 ++nl;
 }

      _LOG(log, logtype::LOGS, "%s.%03d %5d %5d %c %-8s: %s\n",
         timeBuf, entry->nsec / 1000000, entry->pid, entry->tid,
         prioChar, tag, msg);
 } while ((msg = nl));
 }

  android_logger_list_free(logger_list);
}
