status_t Parcel::writeParcelFileDescriptor(int fd, int commChannel) {
 status_t status;

 if (fd < 0) {
        status = writeInt32(0);  
 if (status) return status;
 } else {
        status = writeInt32(1);  
 if (status) return status;
        status = writeDupFileDescriptor(fd);
 if (status) return status;
 if (commChannel < 0) {
            status = writeInt32(0);  
 if (status) return status;
 } else {
            status = writeInt32(1);  
 if (status) return status;
            status = writeDupFileDescriptor(commChannel);
 }
 }
 return status;
}
