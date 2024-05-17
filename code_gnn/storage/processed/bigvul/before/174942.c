void CameraClient::dataCallbackTimestamp(nsecs_t timestamp,
 int32_t msgType, const sp<IMemory>& dataPtr, void* user) {
    LOG2("dataCallbackTimestamp(%d)", msgType);

 Mutex* lock = getClientLockFromCookie(user);
 if (lock == NULL) return;
 Mutex::Autolock alock(*lock);

 CameraClient* client =
 static_cast<CameraClient*>(getClientFromCookie(user));
 if (client == NULL) return;

 if (!client->lockIfMessageWanted(msgType)) return;

 if (dataPtr == 0) {
        ALOGE("Null data returned in data with timestamp callback");
        client->handleGenericNotify(CAMERA_MSG_ERROR, UNKNOWN_ERROR, 0);
 return;
 }

    client->handleGenericDataTimestamp(timestamp, msgType, dataPtr);
}
