status_t Camera2Client::cancelAutoFocus() {
    ATRACE_CALL();
 Mutex::Autolock icl(mBinderSerializationLock);
    ALOGV("%s: Camera %d", __FUNCTION__, mCameraId);
 status_t res;
 if ( (res = checkPid(__FUNCTION__) ) != OK) return res;

 int triggerId;
 {
 SharedParameters::Lock l(mParameters);
 if (l.mParameters.focusMode == Parameters::FOCUS_MODE_FIXED ||
                l.mParameters.focusMode == Parameters::FOCUS_MODE_INFINITY) {
 return OK;
 }

 if (l.mParameters.afTriggerCounter == l.mParameters.currentAfTriggerId) {
            ATRACE_ASYNC_END(kAutofocusLabel, l.mParameters.currentAfTriggerId);
 }

        triggerId = ++l.mParameters.afTriggerCounter;

 if (l.mParameters.shadowFocusMode != Parameters::FOCUS_MODE_INVALID) {
            ALOGV("%s: Quirk: Restoring focus mode to %d", __FUNCTION__,
                    l.mParameters.shadowFocusMode);
            l.mParameters.focusMode = l.mParameters.shadowFocusMode;
            l.mParameters.shadowFocusMode = Parameters::FOCUS_MODE_INVALID;
            updateRequests(l.mParameters);

 return OK;
 }
 }
    syncWithDevice();

    mDevice->triggerCancelAutofocus(triggerId);

 return OK;
}
