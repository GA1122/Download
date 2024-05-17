static void activity_manager_write(int pid, int signal, int amfd, const std::string& amfd_data) {
 if (amfd == -1) {
 return;
 }

 uint32_t datum = htonl(pid);
 if (!android::base::WriteFully(amfd, &datum, 4)) {
    ALOGE("AM pid write failed: %s\n", strerror(errno));
 return;
 }
  datum = htonl(signal);
 if (!android::base::WriteFully(amfd, &datum, 4)) {
    ALOGE("AM signal write failed: %s\n", strerror(errno));
 return;
 }

 if (!android::base::WriteFully(amfd, amfd_data.c_str(), amfd_data.size())) {
    ALOGE("AM data write failed: %s\n", strerror(errno));
 return;
 }

 uint8_t eodMarker = 0;
 if (!android::base::WriteFully(amfd, &eodMarker, 1)) {
    ALOGE("AM eod write failed: %s\n", strerror(errno));
 return;
 }
  android::base::ReadFully(amfd, &eodMarker, 1);
}
