void Camera2Client::notifyAutoFocus(uint8_t newState, int triggerId) {
    ALOGV("%s: Autofocus state now %d, last trigger %d",
            __FUNCTION__, newState, triggerId);
 bool sendCompletedMessage = false;
 bool sendMovingMessage = false;

 bool success = false;
 bool afInMotion = false;
 {
 SharedParameters::Lock l(mParameters);
 char tmp[32];
 if (l.mParameters.afStateCounter > 0) {
            camera_metadata_enum_snprint(
                ANDROID_CONTROL_AF_STATE, l.mParameters.focusState, tmp, sizeof(tmp));
            ATRACE_ASYNC_END(tmp, l.mParameters.afStateCounter);
 }

        l.mParameters.focusState = newState;
        l.mParameters.afStateCounter++;


        camera_metadata_enum_snprint(
            ANDROID_CONTROL_AF_STATE, l.mParameters.focusState, tmp, sizeof(tmp));
        ATRACE_ASYNC_BEGIN(tmp, l.mParameters.afStateCounter);

 switch (l.mParameters.focusMode) {
 case Parameters::FOCUS_MODE_AUTO:
 case Parameters::FOCUS_MODE_MACRO:
 if (triggerId != l.mParameters.currentAfTriggerId) break;
 switch (newState) {
 case ANDROID_CONTROL_AF_STATE_FOCUSED_LOCKED:
                        success = true;
 case ANDROID_CONTROL_AF_STATE_NOT_FOCUSED_LOCKED:
                        sendCompletedMessage = true;
                        l.mParameters.currentAfTriggerId = -1;
 break;
 case ANDROID_CONTROL_AF_STATE_ACTIVE_SCAN:
 break;
 case ANDROID_CONTROL_AF_STATE_INACTIVE:
 case ANDROID_CONTROL_AF_STATE_PASSIVE_SCAN:
 case ANDROID_CONTROL_AF_STATE_PASSIVE_FOCUSED:
 case ANDROID_CONTROL_AF_STATE_PASSIVE_UNFOCUSED:
 default:
                        ALOGE("%s: Unexpected AF state transition in AUTO/MACRO mode: %d",
                                __FUNCTION__, newState);
 break;
 }
 break;
 case Parameters::FOCUS_MODE_CONTINUOUS_VIDEO:
 case Parameters::FOCUS_MODE_CONTINUOUS_PICTURE:
 switch (newState) {
 case ANDROID_CONTROL_AF_STATE_FOCUSED_LOCKED:
                        success = true;
 case ANDROID_CONTROL_AF_STATE_NOT_FOCUSED_LOCKED:
 if (triggerId != l.mParameters.currentAfTriggerId) break;
                        sendCompletedMessage = true;
                        afInMotion = false;
 if (l.mParameters.enableFocusMoveMessages &&
                                l.mParameters.afInMotion) {
                            sendMovingMessage = true;
 }
                        l.mParameters.currentAfTriggerId = -1;
 break;
 case ANDROID_CONTROL_AF_STATE_INACTIVE:
                        afInMotion = false;
 if (l.mParameters.enableFocusMoveMessages &&
                                l.mParameters.afInMotion) {
                            sendMovingMessage = true;
 }
 break;
 case ANDROID_CONTROL_AF_STATE_PASSIVE_SCAN:
                        afInMotion = true;
 case ANDROID_CONTROL_AF_STATE_PASSIVE_FOCUSED:
 case ANDROID_CONTROL_AF_STATE_PASSIVE_UNFOCUSED:
 if (l.mParameters.enableFocusMoveMessages) {
                            sendMovingMessage = true;
 }
 break;
 }
                l.mParameters.afInMotion = afInMotion;
 break;
 case Parameters::FOCUS_MODE_EDOF:
 case Parameters::FOCUS_MODE_INFINITY:
 case Parameters::FOCUS_MODE_FIXED:
 default:
 if (newState != ANDROID_CONTROL_AF_STATE_INACTIVE) {
                    ALOGE("%s: Unexpected AF state change %d "
 "(ID %d) in focus mode %d",
                          __FUNCTION__, newState, triggerId,
                            l.mParameters.focusMode);
 }
 }
 }
 if (sendMovingMessage) {
 SharedCameraCallbacks::Lock l(mSharedCameraCallbacks);
 if (l.mRemoteCallback != 0) {
            l.mRemoteCallback->notifyCallback(CAMERA_MSG_FOCUS_MOVE,
                    afInMotion ? 1 : 0, 0);
 }
 }
 if (sendCompletedMessage) {
        ATRACE_ASYNC_END(kAutofocusLabel, triggerId);
 SharedCameraCallbacks::Lock l(mSharedCameraCallbacks);
 if (l.mRemoteCallback != 0) {
            l.mRemoteCallback->notifyCallback(CAMERA_MSG_FOCUS,
                    success ? 1 : 0, 0);
 }
 }
}
