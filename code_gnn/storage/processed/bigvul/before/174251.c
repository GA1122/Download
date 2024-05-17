bool Camera3Device::processPartial3AResult(
 uint32_t frameNumber,
 const CameraMetadata& partial, const CaptureResultExtras& resultExtras) {


 bool gotAllStates = true;

 uint8_t afMode;
 uint8_t awbMode;
 uint8_t aeState;
 uint8_t afState;
 uint8_t awbState;

    gotAllStates &= get3AResult(partial, ANDROID_CONTROL_AF_MODE,
 &afMode, frameNumber);

    gotAllStates &= get3AResult(partial, ANDROID_CONTROL_AWB_MODE,
 &awbMode, frameNumber);

    gotAllStates &= get3AResult(partial, ANDROID_CONTROL_AE_STATE,
 &aeState, frameNumber);

    gotAllStates &= get3AResult(partial, ANDROID_CONTROL_AF_STATE,
 &afState, frameNumber);

    gotAllStates &= get3AResult(partial, ANDROID_CONTROL_AWB_STATE,
 &awbState, frameNumber);

 if (!gotAllStates) return false;

    ALOGVV("%s: Camera %d: Frame %d, Request ID %d: AF mode %d, AWB mode %d, "
 "AF state %d, AE state %d, AWB state %d, "
 "AF trigger %d, AE precapture trigger %d",
        __FUNCTION__, mId, frameNumber, resultExtras.requestId,
        afMode, awbMode,
        afState, aeState, awbState,
        resultExtras.afTriggerId, resultExtras.precaptureTriggerId);


 const size_t kMinimal3AResultEntries = 10;

 Mutex::Autolock l(mOutputLock);

 CaptureResult captureResult;
    captureResult.mResultExtras = resultExtras;
    captureResult.mMetadata = CameraMetadata(kMinimal3AResultEntries,   0);
 CaptureResult& min3AResult =
 *mResultQueue.insert(mResultQueue.end(), captureResult);

 if (!insert3AResult(min3AResult.mMetadata, ANDROID_REQUEST_FRAME_COUNT,
 reinterpret_cast<int32_t*>(&frameNumber), frameNumber)) {
 return false;
 }

 int32_t requestId = resultExtras.requestId;
 if (!insert3AResult(min3AResult.mMetadata, ANDROID_REQUEST_ID,
 &requestId, frameNumber)) {
 return false;
 }

 if (mDeviceVersion < CAMERA_DEVICE_API_VERSION_3_2) {
 static const uint8_t partialResult = ANDROID_QUIRKS_PARTIAL_RESULT_PARTIAL;
 if (!insert3AResult(min3AResult.mMetadata, ANDROID_QUIRKS_PARTIAL_RESULT,
 &partialResult, frameNumber)) {
 return false;
 }
 }

 if (!insert3AResult(min3AResult.mMetadata, ANDROID_CONTROL_AF_MODE,
 &afMode, frameNumber)) {
 return false;
 }

 if (!insert3AResult(min3AResult.mMetadata, ANDROID_CONTROL_AWB_MODE,
 &awbMode, frameNumber)) {
 return false;
 }

 if (!insert3AResult(min3AResult.mMetadata, ANDROID_CONTROL_AE_STATE,
 &aeState, frameNumber)) {
 return false;
 }

 if (!insert3AResult(min3AResult.mMetadata, ANDROID_CONTROL_AF_STATE,
 &afState, frameNumber)) {
 return false;
 }

 if (!insert3AResult(min3AResult.mMetadata, ANDROID_CONTROL_AWB_STATE,
 &awbState, frameNumber)) {
 return false;
 }

 if (!insert3AResult(min3AResult.mMetadata, ANDROID_CONTROL_AF_TRIGGER_ID,
 &resultExtras.afTriggerId, frameNumber)) {
 return false;
 }

 if (!insert3AResult(min3AResult.mMetadata, ANDROID_CONTROL_AE_PRECAPTURE_ID,
 &resultExtras.precaptureTriggerId, frameNumber)) {
 return false;
 }

    mResultSignal.signal();

 return true;
}
