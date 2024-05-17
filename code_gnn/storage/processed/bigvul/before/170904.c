void OMXNodeInstance::invalidateBufferID(OMX::buffer_id buffer) {
 if (buffer == 0) {
 return;
 }
 Mutex::Autolock autoLock(mBufferIDLock);
 ssize_t index = mBufferIDToBufferHeader.indexOfKey(buffer);
 if (index < 0) {
        CLOGW("invalidateBufferID: buffer %u not found", buffer);
 return;
 }
    mBufferHeaderToBufferID.removeItem(mBufferIDToBufferHeader.valueAt(index));
    mBufferIDToBufferHeader.removeItemsAt(index);
}
