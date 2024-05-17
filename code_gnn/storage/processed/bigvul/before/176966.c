void InputDispatcher::logDispatchStateLocked() {
 String8 dump;
    dumpDispatchStateLocked(dump);

 char* text = dump.lockBuffer(dump.size());
 char* start = text;
 while (*start != '\0') {
 char* end = strchr(start, '\n');
 if (*end == '\n') {
 *(end++) = '\0';
 }
        ALOGD("%s", start);
        start = end;
 }
}
