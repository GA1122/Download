status_t MediaPlayerService::dump(int fd, const Vector<String16>& args)
{
 const size_t SIZE = 256;
 char buffer[SIZE];
 String8 result;
 SortedVector< sp<Client> > clients;  
 SortedVector< sp<MediaRecorderClient> > mediaRecorderClients;

 if (checkCallingPermission(String16("android.permission.DUMP")) == false) {
        snprintf(buffer, SIZE, "Permission Denial: "
 "can't dump MediaPlayerService from pid=%d, uid=%d\n",
 IPCThreadState::self()->getCallingPid(),
 IPCThreadState::self()->getCallingUid());
        result.append(buffer);
 } else {
 Mutex::Autolock lock(mLock);
 for (int i = 0, n = mClients.size(); i < n; ++i) {
            sp<Client> c = mClients[i].promote();
 if (c != 0) c->dump(fd, args);
            clients.add(c);
 }
 if (mMediaRecorderClients.size() == 0) {
                result.append(" No media recorder client\n\n");
 } else {
 for (int i = 0, n = mMediaRecorderClients.size(); i < n; ++i) {
                sp<MediaRecorderClient> c = mMediaRecorderClients[i].promote();
 if (c != 0) {
                    snprintf(buffer, 255, " MediaRecorderClient pid(%d)\n", c->mPid);
                    result.append(buffer);
                    write(fd, result.string(), result.size());
                    result = "\n";
                    c->dump(fd, args);
                    mediaRecorderClients.add(c);
 }
 }
 }

        result.append(" Files opened and/or mapped:\n");
        snprintf(buffer, SIZE, "/proc/%d/maps", getpid());
 FILE *f = fopen(buffer, "r");
 if (f) {
 while (!feof(f)) {
                fgets(buffer, SIZE, f);
 if (strstr(buffer, " /storage/") ||
                    strstr(buffer, " /system/sounds/") ||
                    strstr(buffer, " /data/") ||
                    strstr(buffer, " /system/media/")) {
                    result.append("  ");
                    result.append(buffer);
 }
 }
            fclose(f);
 } else {
            result.append("couldn't open ");
            result.append(buffer);
            result.append("\n");
 }

        snprintf(buffer, SIZE, "/proc/%d/fd", getpid());
 DIR *d = opendir(buffer);
 if (d) {
 struct dirent *ent;
 while((ent = readdir(d)) != NULL) {
 if (strcmp(ent->d_name,".") && strcmp(ent->d_name,"..")) {
                    snprintf(buffer, SIZE, "/proc/%d/fd/%s", getpid(), ent->d_name);
 struct stat s;
 if (lstat(buffer, &s) == 0) {
 if ((s.st_mode & S_IFMT) == S_IFLNK) {
 char linkto[256];
 int len = readlink(buffer, linkto, sizeof(linkto));
 if(len > 0) {
 if(len > 255) {
                                    linkto[252] = '.';
                                    linkto[253] = '.';
                                    linkto[254] = '.';
                                    linkto[255] = 0;
 } else {
                                    linkto[len] = 0;
 }
 if (strstr(linkto, "/storage/") == linkto ||
                                    strstr(linkto, "/system/sounds/") == linkto ||
                                    strstr(linkto, "/data/") == linkto ||
                                    strstr(linkto, "/system/media/") == linkto) {
                                    result.append("  ");
                                    result.append(buffer);
                                    result.append(" -> ");
                                    result.append(linkto);
                                    result.append("\n");
 }
 }
 } else {
                            result.append("  unexpected type for ");
                            result.append(buffer);
                            result.append("\n");
 }
 }
 }
 }
            closedir(d);
 } else {
            result.append("couldn't open ");
            result.append(buffer);
            result.append("\n");
 }

        gLooperRoster.dump(fd, args);

 bool dumpMem = false;
 bool unreachableMemory = false;
 for (size_t i = 0; i < args.size(); i++) {
 if (args[i] == String16("-m")) {
                dumpMem = true;
 } else if (args[i] == String16("--unreachable")) {
                unreachableMemory = true;
 }
 }
 if (dumpMem) {
            result.append("\nDumping memory:\n");
            std::string s = dumpMemoryAddresses(100  );
            result.append(s.c_str(), s.size());
 }
 if (unreachableMemory) {
            result.append("\nDumping unreachable memory:\n");
            std::string s = GetUnreachableMemoryString(true  , 10000  );
            result.append(s.c_str(), s.size());
 }
 }
    write(fd, result.string(), result.size());
 return NO_ERROR;
}
