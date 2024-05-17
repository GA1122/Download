status_t Parcel::writeBlob(size_t len, bool mutableCopy, WritableBlob* outBlob)
{
 if (len > INT32_MAX) {
 return BAD_VALUE;
 }

 status_t status;
 if (!mAllowFds || len <= BLOB_INPLACE_LIMIT) {
        ALOGV("writeBlob: write in place");
        status = writeInt32(BLOB_INPLACE);
 if (status) return status;

 void* ptr = writeInplace(len);
 if (!ptr) return NO_MEMORY;

        outBlob->init(-1, ptr, len, false);
 return NO_ERROR;
 }

    ALOGV("writeBlob: write to ashmem");
 int fd = ashmem_create_region("Parcel Blob", len);
 if (fd < 0) return NO_MEMORY;

 int result = ashmem_set_prot_region(fd, PROT_READ | PROT_WRITE);
 if (result < 0) {
        status = result;
 } else {
 void* ptr = ::mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
 if (ptr == MAP_FAILED) {
            status = -errno;
 } else {
 if (!mutableCopy) {
                result = ashmem_set_prot_region(fd, PROT_READ);
 }
 if (result < 0) {
                status = result;
 } else {
                status = writeInt32(mutableCopy ? BLOB_ASHMEM_MUTABLE : BLOB_ASHMEM_IMMUTABLE);
 if (!status) {
                    status = writeFileDescriptor(fd, true  );
 if (!status) {
                        outBlob->init(fd, ptr, len, mutableCopy);
 return NO_ERROR;
 }
 }
 }
 }
 ::munmap(ptr, len);
 }
 ::close(fd);
 return status;
}
