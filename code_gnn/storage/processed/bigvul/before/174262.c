void Camera3Device::sendCaptureResult(CameraMetadata &pendingMetadata,
 CaptureResultExtras &resultExtras,
 CameraMetadata &collectedPartialResult,
 uint32_t frameNumber,
 bool reprocess,
 const AeTriggerCancelOverride_t &aeTriggerCancelOverride) {
 if (pendingMetadata.isEmpty())
 return;

 Mutex::Autolock l(mOutputLock);

 if (reprocess) {
 if (frameNumber < mNextReprocessResultFrameNumber) {
            SET_ERR("Out-of-order reprocess capture result metadata submitted! "
 "(got frame number %d, expecting %d)",
                frameNumber, mNextReprocessResultFrameNumber);
 return;
 }
        mNextReprocessResultFrameNumber = frameNumber + 1;
 } else {
 if (frameNumber < mNextResultFrameNumber) {
            SET_ERR("Out-of-order capture result metadata submitted! "
 "(got frame number %d, expecting %d)",
                    frameNumber, mNextResultFrameNumber);
 return;
 }
        mNextResultFrameNumber = frameNumber + 1;
 }

 CaptureResult captureResult;
    captureResult.mResultExtras = resultExtras;
    captureResult.mMetadata = pendingMetadata;

 if (captureResult.mMetadata.update(ANDROID_REQUEST_FRAME_COUNT,
 (int32_t*)&frameNumber, 1) != OK) {
        SET_ERR("Failed to set frame# in metadata (%d)",
                frameNumber);
 return;
 } else {
        ALOGVV("%s: Camera %d: Set frame# in metadata (%d)",
                __FUNCTION__, mId, frameNumber);
 }

 if (mUsePartialResult && !collectedPartialResult.isEmpty()) {
        captureResult.mMetadata.append(collectedPartialResult);
 }

    captureResult.mMetadata.sort();

    camera_metadata_entry entry =
            captureResult.mMetadata.find(ANDROID_SENSOR_TIMESTAMP);
 if (entry.count == 0) {
        SET_ERR("No timestamp provided by HAL for frame %d!",
                frameNumber);
 return;
 }

    overrideResultForPrecaptureCancel(&captureResult.mMetadata, aeTriggerCancelOverride);

 List<CaptureResult>::iterator queuedResult =
            mResultQueue.insert(mResultQueue.end(), CaptureResult(captureResult));
    ALOGVV("%s: result requestId = %" PRId32 ", frameNumber = %" PRId64
 ", burstId = %" PRId32, __FUNCTION__,
           queuedResult->mResultExtras.requestId,
           queuedResult->mResultExtras.frameNumber,
           queuedResult->mResultExtras.burstId);

    mResultSignal.signal();
}
