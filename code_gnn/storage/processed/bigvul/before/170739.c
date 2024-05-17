void OMXNodeInstance::invalidateBufferID(OMX::buffer_id buffer) {
 if (buffer == 0) {
 return;
 }
 Mutex::Autolock autoLock(mBufferIDLock);
    mBufferHeaderToBufferID.removeItem(mBufferIDToBufferHeader.valueFor(buffer));

     mBufferIDToBufferHeader.removeItem(buffer);
 }
