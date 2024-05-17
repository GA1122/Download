void CameraClient::dataCallback(int32_t msgType,
 const sp<IMemory>& dataPtr, camera_frame_metadata_t *metadata, void* user) {
    LOG2("dataCallback(%d)", msgType);

 Mutex* lock = getClientLockFromCookie(user);
 if (lock == NULL) return;
 Mutex::Autolock alock(*lock);

 CameraClient* client =
 static_cast<CameraClient*>(getClientFromCookie(user));
 if (client == NULL) return;

 if (!client->lockIfMessageWanted(msgType)) return;
 if (dataPtr == 0 && metadata == NULL) {
        ALOGE("Null data returned in data callback");
        client->handleGenericNotify(CAMERA_MSG_ERROR, UNKNOWN_ERROR, 0);
 return;
 }

 switch (msgType & ~CAMERA_MSG_PREVIEW_METADATA) {
 case CAMERA_MSG_PREVIEW_FRAME:
            client->handlePreviewData(msgType, dataPtr, metadata);
 break;
 case CAMERA_MSG_POSTVIEW_FRAME:
            client->handlePostview(dataPtr);
 break;
 case CAMERA_MSG_RAW_IMAGE:
            client->handleRawPicture(dataPtr);
 break;
 case CAMERA_MSG_COMPRESSED_IMAGE:
            client->handleCompressedPicture(dataPtr);
 break;
 default:
            client->handleGenericData(msgType, dataPtr, metadata);
 break;
 }
}
