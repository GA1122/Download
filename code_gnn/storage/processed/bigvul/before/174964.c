status_t CameraClient::sendCommand(int32_t cmd, int32_t arg1, int32_t arg2) {
    LOG1("sendCommand (pid %d)", getCallingPid());
 int orientation;
 Mutex::Autolock lock(mLock);
 status_t result = checkPidAndHardware();
 if (result != NO_ERROR) return result;

 if (cmd == CAMERA_CMD_SET_DISPLAY_ORIENTATION) {
        orientation = getOrientation(arg1, mCameraFacing == CAMERA_FACING_FRONT);
 if (orientation == -1) return BAD_VALUE;

 if (mOrientation != orientation) {
            mOrientation = orientation;
 if (mPreviewWindow != 0) {
                native_window_set_buffers_transform(mPreviewWindow.get(),
                        mOrientation);
 }
 }
 return OK;
 } else if (cmd == CAMERA_CMD_ENABLE_SHUTTER_SOUND) {
 switch (arg1) {
 case 0:
 return enableShutterSound(false);
 case 1:
 return enableShutterSound(true);
 default:
 return BAD_VALUE;
 }
 return OK;
 } else if (cmd == CAMERA_CMD_PLAY_RECORDING_SOUND) {
        mCameraService->playSound(CameraService::SOUND_RECORDING);
 } else if (cmd == CAMERA_CMD_SET_VIDEO_BUFFER_COUNT) {
 return INVALID_OPERATION;
 } else if (cmd == CAMERA_CMD_PING) {
 return OK;
 }

 return mHardware->sendCommand(cmd, arg1, arg2);
}
