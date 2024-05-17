void Camera2Client::setPreviewCallbackFlagL(Parameters &params, int flag) {
 status_t res = OK;

 switch(params.state) {
 case Parameters::STOPPED:
 case Parameters::WAITING_FOR_PREVIEW_WINDOW:
 case Parameters::PREVIEW:
 case Parameters::STILL_CAPTURE:
 break;
 default:
 if (flag & CAMERA_FRAME_CALLBACK_FLAG_ENABLE_MASK) {
                ALOGE("%s: Camera %d: Can't use preview callbacks "
 "in state %d", __FUNCTION__, mCameraId, params.state);
 return;
 }
 }

 if (flag & CAMERA_FRAME_CALLBACK_FLAG_ONE_SHOT_MASK) {
        ALOGV("%s: setting oneshot", __FUNCTION__);
        params.previewCallbackOneShot = true;
 }
 if (params.previewCallbackFlags != (uint32_t)flag) {

 if (params.previewCallbackSurface && flag != CAMERA_FRAME_CALLBACK_FLAG_NOOP) {
            res = mCallbackProcessor->setCallbackWindow(NULL);
 if (res != OK) {
                ALOGE("%s: Camera %d: Unable to clear preview callback surface:"
 " %s (%d)", __FUNCTION__, mCameraId, strerror(-res), res);
 return;
 }
            params.previewCallbackSurface = false;
 }

        params.previewCallbackFlags = flag;

 if (params.state == Parameters::PREVIEW) {
            res = startPreviewL(params, true);
 if (res != OK) {
                ALOGE("%s: Camera %d: Unable to refresh request in state %s",
                        __FUNCTION__, mCameraId,
 Parameters::getStateName(params.state));
 }
 }
 }
}
