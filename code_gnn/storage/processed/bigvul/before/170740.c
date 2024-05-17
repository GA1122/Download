 OMX::buffer_id OMXNodeInstance::makeBufferID(OMX_BUFFERHEADERTYPE *bufferHeader) {
     if (bufferHeader == NULL) {
         return 0;
 }
 Mutex::Autolock autoLock(mBufferIDLock);
    OMX::buffer_id buffer;
 do {  
 if (++mBufferIDCount == 0) {
 ++mBufferIDCount;
 }
        buffer = (OMX::buffer_id)mBufferIDCount;
 } while (mBufferIDToBufferHeader.indexOfKey(buffer) >= 0);
    mBufferIDToBufferHeader.add(buffer, bufferHeader);
    mBufferHeaderToBufferID.add(bufferHeader, buffer);
 return buffer;
}
