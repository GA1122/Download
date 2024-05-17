void CameraClient::notifyCallback(int32_t msgType, int32_t ext1,
 int32_t ext2, void* user) {
    LOG2("notifyCallback(%d)", msgType);

 Mutex* lock = getClientLockFromCookie(user);
 if (lock == NULL) return;
 Mutex::Autolock alock(*lock);

 CameraClient* client =
 static_cast<CameraClient*>(getClientFromCookie(user));
 if (client == NULL) return;

 if (!client->lockIfMessageWanted(msgType)) return;

 switch (msgType) {
 case CAMERA_MSG_SHUTTER:
            client->handleShutter();
 break;
 default:
            client->handleGenericNotify(msgType, ext1, ext2);
 break;
 }
}
