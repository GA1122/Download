status_t CameraClient::takePicture(int msgType) {
    LOG1("takePicture (pid %d): 0x%x", getCallingPid(), msgType);

 Mutex::Autolock lock(mLock);
 status_t result = checkPidAndHardware();
 if (result != NO_ERROR) return result;

 if ((msgType & CAMERA_MSG_RAW_IMAGE) &&
 (msgType & CAMERA_MSG_RAW_IMAGE_NOTIFY)) {
        ALOGE("CAMERA_MSG_RAW_IMAGE and CAMERA_MSG_RAW_IMAGE_NOTIFY"
 " cannot be both enabled");
 return BAD_VALUE;
 }

 int picMsgType = msgType
 & (CAMERA_MSG_SHUTTER |
                           CAMERA_MSG_POSTVIEW_FRAME |
                           CAMERA_MSG_RAW_IMAGE |
                           CAMERA_MSG_RAW_IMAGE_NOTIFY |
                           CAMERA_MSG_COMPRESSED_IMAGE);

    enableMsgType(picMsgType);

 return mHardware->takePicture();
}
