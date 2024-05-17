status_t Camera2Client::sendCommand(int32_t cmd, int32_t arg1, int32_t arg2) {
    ATRACE_CALL();
 Mutex::Autolock icl(mBinderSerializationLock);
 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

    ALOGV("%s: Camera %d: Command %d (%d, %d)", __FUNCTION__, mCameraId,
            cmd, arg1, arg2);

 switch (cmd) {
 case CAMERA_CMD_START_SMOOTH_ZOOM:
 return commandStartSmoothZoomL();
 case CAMERA_CMD_STOP_SMOOTH_ZOOM:
 return commandStopSmoothZoomL();
 case CAMERA_CMD_SET_DISPLAY_ORIENTATION:
 return commandSetDisplayOrientationL(arg1);
 case CAMERA_CMD_ENABLE_SHUTTER_SOUND:
 return commandEnableShutterSoundL(arg1 == 1);
 case CAMERA_CMD_PLAY_RECORDING_SOUND:
 return commandPlayRecordingSoundL();
 case CAMERA_CMD_START_FACE_DETECTION:
 return commandStartFaceDetectionL(arg1);
 case CAMERA_CMD_STOP_FACE_DETECTION: {
 SharedParameters::Lock l(mParameters);
 return commandStopFaceDetectionL(l.mParameters);
 }
 case CAMERA_CMD_ENABLE_FOCUS_MOVE_MSG:
 return commandEnableFocusMoveMsgL(arg1 == 1);
 case CAMERA_CMD_PING:
 return commandPingL();
 case CAMERA_CMD_SET_VIDEO_BUFFER_COUNT:
 return commandSetVideoBufferCountL(arg1);
 default:
            ALOGE("%s: Unknown command %d (arguments %d, %d)",
                    __FUNCTION__, cmd, arg1, arg2);
 return BAD_VALUE;
 }
}
