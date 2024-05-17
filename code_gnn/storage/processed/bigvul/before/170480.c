int Parcel::readParcelFileDescriptor(int& outCommChannel) const {
 int fd;
    outCommChannel = -1;

 if (readInt32() == 0) {
        fd = -1;
 } else {
        fd = readFileDescriptor();
 if (fd >= 0 && readInt32() != 0) {
            outCommChannel = readFileDescriptor();
 }
 }
 return fd;
}
