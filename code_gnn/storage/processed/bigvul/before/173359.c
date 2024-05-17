void Parcel::ipcSetDataReference(const uint8_t* data, size_t dataSize,
 const binder_size_t* objects, size_t objectsCount, release_func relFunc, void* relCookie)
{
 binder_size_t minOffset = 0;
    freeDataNoInit();
    mError = NO_ERROR;
    mData = const_cast<uint8_t*>(data);
    mDataSize = mDataCapacity = dataSize;
    mDataPos = 0;
    ALOGV("setDataReference Setting data pos of %p to %zu", this, mDataPos);
    mObjects = const_cast<binder_size_t*>(objects);
    mObjectsSize = mObjectsCapacity = objectsCount;
    mNextObjectHint = 0;
    mOwner = relFunc;
    mOwnerCookie = relCookie;
 for (size_t i = 0; i < mObjectsSize; i++) {
 binder_size_t offset = mObjects[i];
 if (offset < minOffset) {
            ALOGE("%s: bad object offset %" PRIu64 " < %" PRIu64 "\n",
                  __func__, (uint64_t)offset, (uint64_t)minOffset);
            mObjectsSize = 0;
 break;
 }
        minOffset = offset + sizeof(flat_binder_object);
 }
    scanForFds();
}
