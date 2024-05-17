static int activity_manager_connect() {
  android::base::unique_fd amfd(socket(PF_UNIX, SOCK_STREAM, 0));
 if (amfd.get() < -1) {
    ALOGE("debuggerd: Unable to connect to activity manager (socket failed: %s)", strerror(errno));
 return -1;
 }

 struct sockaddr_un address;
  memset(&address, 0, sizeof(address));
  address.sun_family = AF_UNIX;
  strncpy(address.sun_path, "/data/system/ndebugsocket", sizeof(address.sun_path));
 if (TEMP_FAILURE_RETRY(connect(amfd.get(), reinterpret_cast<struct sockaddr*>(&address),
 sizeof(address))) == -1) {
    ALOGE("debuggerd: Unable to connect to activity manager (connect failed: %s)", strerror(errno));
 return -1;
 }

 struct timeval tv;
  memset(&tv, 0, sizeof(tv));
  tv.tv_sec = 1;  
 if (setsockopt(amfd.get(), SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == -1) {
    ALOGE("debuggerd: Unable to connect to activity manager (setsockopt SO_SNDTIMEO failed: %s)",
          strerror(errno));
 return -1;
 }

  tv.tv_sec = 3;  
 if (setsockopt(amfd.get(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
    ALOGE("debuggerd: Unable to connect to activity manager (setsockopt SO_RCVTIMEO failed: %s)",
          strerror(errno));
 return -1;
 }

 return amfd.release();
}
