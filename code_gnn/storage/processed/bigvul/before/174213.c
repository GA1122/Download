status_t Camera3Device::deleteStream(int id) {
    ATRACE_CALL();
 Mutex::Autolock il(mInterfaceLock);
 Mutex::Autolock l(mLock);
 status_t res;

    ALOGV("%s: Camera %d: Deleting stream %d", __FUNCTION__, mId, id);

 if (mStatus == STATUS_ACTIVE) {
        ALOGV("%s: Camera %d: Device not idle", __FUNCTION__, mId);
 return -EBUSY;
 }

    sp<Camera3StreamInterface> deletedStream;
 ssize_t outputStreamIdx = mOutputStreams.indexOfKey(id);
 if (mInputStream != NULL && id == mInputStream->getId()) {
        deletedStream = mInputStream;
        mInputStream.clear();
 } else {
 if (outputStreamIdx == NAME_NOT_FOUND) {
            CLOGE("Stream %d does not exist", id);
 return BAD_VALUE;
 }
 }

 if (outputStreamIdx != NAME_NOT_FOUND) {
        deletedStream = mOutputStreams.editValueAt(outputStreamIdx);
        mOutputStreams.removeItem(id);
 }

    res = deletedStream->disconnect();
 if (res != OK) {
        SET_ERR_L("Can't disconnect deleted stream %d", id);
 }
    mDeletedStreams.add(deletedStream);
    mNeedConfig = true;

 return res;
}
