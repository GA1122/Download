void BpMemoryHeap::assertReallyMapped() const
{
 if (mHeapId == -1) {


 Parcel data, reply;
        data.writeInterfaceToken(IMemoryHeap::getInterfaceDescriptor());
 status_t err = remote()->transact(HEAP_ID, data, &reply);
 int parcel_fd = reply.readFileDescriptor();
 ssize_t size = reply.readInt32();
 uint32_t flags = reply.readInt32();
 uint32_t offset = reply.readInt32();

        ALOGE_IF(err, "binder=%p transaction failed fd=%d, size=%zd, err=%d (%s)",
                asBinder().get(), parcel_fd, size, err, strerror(-err));

 int fd = dup( parcel_fd );
        ALOGE_IF(fd==-1, "cannot dup fd=%d, size=%zd, err=%d (%s)",
                parcel_fd, size, err, strerror(errno));

 int access = PROT_READ;
 if (!(flags & READ_ONLY)) {
            access |= PROT_WRITE;
 }

 Mutex::Autolock _l(mLock);
 if (mHeapId == -1) {
            mRealHeap = true;
            mBase = mmap(0, size, access, MAP_SHARED, fd, offset);
 if (mBase == MAP_FAILED) {
                ALOGE("cannot map BpMemoryHeap (binder=%p), size=%zd, fd=%d (%s)",
                        asBinder().get(), size, fd, strerror(errno));
                close(fd);
 } else {
                mSize = size;
                mFlags = flags;
                mOffset = offset;
                android_atomic_write(fd, &mHeapId);
 }
 }
 }
}
