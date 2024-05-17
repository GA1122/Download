status_t Parcel::writeDupImmutableBlobFileDescriptor(int fd)
{
 if (!mAllowFds) return FDS_NOT_ALLOWED;
 status_t status = writeInt32(BLOB_ASHMEM_IMMUTABLE);
 if (status) return status;
 return writeDupFileDescriptor(fd);
}
