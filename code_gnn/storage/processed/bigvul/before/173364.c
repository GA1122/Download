status_t Parcel::readBlob(size_t len, ReadableBlob* outBlob) const
{
 int32_t blobType;
 status_t status = readInt32(&blobType);
 if (status) return status;

 if (blobType == BLOB_INPLACE) {
        ALOGV("readBlob: read in place");
 const void* ptr = readInplace(len);
 if (!ptr) return BAD_VALUE;

        outBlob->init(-1, const_cast<void*>(ptr), len, false);
 return NO_ERROR;
 }

    ALOGV("readBlob: read from ashmem");
 bool isMutable = (blobType == BLOB_ASHMEM_MUTABLE);
 int fd = readFileDescriptor();
 if (fd == int(BAD_TYPE)) return BAD_VALUE;

 void* ptr = ::mmap(NULL, len, isMutable ? PROT_READ | PROT_WRITE : PROT_READ,
            MAP_SHARED, fd, 0);
 if (ptr == MAP_FAILED) return NO_MEMORY;

    outBlob->init(fd, ptr, len, isMutable);
 return NO_ERROR;
}
