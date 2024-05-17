int dexOptCreateEmptyHeader(int fd)
{
 DexOptHeader optHdr;
 ssize_t actual;

    assert(lseek(fd, 0, SEEK_CUR) == 0);

  
    assert((sizeof(optHdr) & 0x07) == 0);
    memset(&optHdr, 0xff, sizeof(optHdr));
    optHdr.dexOffset = sizeof(optHdr);
    actual = write(fd, &optHdr, sizeof(optHdr));
 if (actual != sizeof(optHdr)) {
 int err = errno ? errno : -1;
        ALOGE("opt header write failed: %s", strerror(errno));
 return errno;
 }

 return 0;
}
