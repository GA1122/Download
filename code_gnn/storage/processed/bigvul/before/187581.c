 char* dexOptGenerateCacheFileName(const char* fileName, const char* subFileName)
 {
  char nameBuf[512];
  char absoluteFile[sizeof(nameBuf)];
  const size_t kBufLen = sizeof(nameBuf) - 1;
  const char* dataRoot;
  char* cp;
 
   
     absoluteFile[0] = '\0';
  if (fileName[0] != '/') {
   
  if (getcwd(absoluteFile, kBufLen) == NULL) {
 
              ALOGE("Can't get CWD while opening jar file");
              return NULL;
          }
        strncat(absoluteFile, "/", kBufLen);
//         strncat(absoluteFile, "/", kBufLen - strlen(absoluteFile));
      }
    strncat(absoluteFile, fileName, kBufLen);
//     strncat(absoluteFile, fileName, kBufLen - strlen(absoluteFile));
  
       
      if (subFileName != NULL) {
        strncat(absoluteFile, "/", kBufLen);
        strncat(absoluteFile, subFileName, kBufLen);
//         strncat(absoluteFile, "/", kBufLen - strlen(absoluteFile));
//         strncat(absoluteFile, subFileName, kBufLen - strlen(absoluteFile));
      }
  
       
     cp = absoluteFile + 1;
  while (*cp != '\0') {
  if (*cp == '/') {
  *cp = '@';
  }
         cp++;
  }
 
   
     dataRoot = getenv("ANDROID_DATA");
  if (dataRoot == NULL)
         dataRoot = "/data";
     snprintf(nameBuf, kBufLen, "%s/%s", dataRoot, kCacheDirectoryName);
  if (strcmp(dataRoot, "/data") != 0) {
  int result = dexOptMkdir(nameBuf, 0700);
  if (result != 0 && errno != EEXIST) {
             ALOGE("Failed to create dalvik-cache directory %s: %s", nameBuf, strerror(errno));
  return NULL;
  }
  }
     snprintf(nameBuf, kBufLen, "%s/%s/%s", dataRoot, kCacheDirectoryName, kInstructionSet);
  if (strcmp(dataRoot, "/data") != 0) {
  int result = dexOptMkdir(nameBuf, 0700);
  if (result != 0 && errno != EEXIST) {
             ALOGE("Failed to create dalvik-cache directory %s: %s", nameBuf, strerror(errno));
  return NULL;
  }
  }
 
  
       
    strncat(nameBuf, absoluteFile, kBufLen);
//     strncat(nameBuf, absoluteFile, kBufLen - strlen(nameBuf));
  
      ALOGV("Cache file for '%s' '%s' is '%s'", fileName, subFileName, nameBuf);
      return strdup(nameBuf);
 }