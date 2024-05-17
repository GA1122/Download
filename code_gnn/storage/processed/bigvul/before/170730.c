OMX::buffer_id OMXNodeInstance::findBufferID(OMX_BUFFERHEADERTYPE *bufferHeader) {
 if (bufferHeader == NULL) {
 return 0;
 }
 Mutex::Autolock autoLock(mBufferIDLock);
 return mBufferHeaderToBufferID.valueFor(bufferHeader);
}
