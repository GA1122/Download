OMX::buffer_id OMXNodeInstance::findBufferID(OMX_BUFFERHEADERTYPE *bufferHeader) {
 if (bufferHeader == NULL) {
 return 0;
 }
 Mutex::Autolock autoLock(mBufferIDLock);
 ssize_t index = mBufferHeaderToBufferID.indexOfKey(bufferHeader);
 if (index < 0) {
        CLOGW("findBufferID: bufferHeader %p not found", bufferHeader);
 return 0;
 }
 return mBufferHeaderToBufferID.valueAt(index);
}
