status_t Parcel::writeDupFileDescriptor(int fd)
{
 int dupFd = dup(fd);
 if (dupFd < 0) {
 return -errno;
 }
 status_t err = writeFileDescriptor(dupFd, true  );
 if (err) {
        close(dupFd);
 }
 return err;
}
