blockingWrite(int fd, const void *buffer, size_t len) {
 size_t writeOffset = 0;
 const uint8_t *toWrite;

    toWrite = (const uint8_t *)buffer;

 while (writeOffset < len) {
 ssize_t written;
 do {
            written = write (fd, toWrite + writeOffset,
                                len - writeOffset);
 } while (written < 0 && ((errno == EINTR) || (errno == EAGAIN)));

 if (written >= 0) {
            writeOffset += written;
 } else {  
            RLOGE ("RIL Response: unexpected error on write errno:%d", errno);
            close(fd);
 return -1;
 }
 }
#if VDBG
    RLOGE("RIL Response bytes written:%d", writeOffset);
#endif
 return 0;
}
