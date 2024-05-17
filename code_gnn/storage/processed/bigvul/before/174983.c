status_t CameraDeviceClient::createDefaultRequest(int templateId,
  
 CameraMetadata* request)
{
    ATRACE_CALL();
    ALOGV("%s (templateId = 0x%x)", __FUNCTION__, templateId);

 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

 Mutex::Autolock icl(mBinderSerializationLock);

 if (!mDevice.get()) return DEAD_OBJECT;

 CameraMetadata metadata;
 if ( (res = mDevice->createDefaultRequest(templateId, &metadata) ) == OK &&
        request != NULL) {

        request->swap(metadata);
 }

 return res;
}
