void Camera3Device::notifyShutter(const camera3_shutter_msg_t &msg,
 NotificationListener *listener) {
 ssize_t idx;

 {
 Mutex::Autolock l(mInFlightLock);
        idx = mInFlightMap.indexOfKey(msg.frame_number);
 if (idx >= 0) {
 InFlightRequest &r = mInFlightMap.editValueAt(idx);

 {
 Mutex::Autolock l(mOutputLock);
 if (r.hasInputBuffer) {
 if (msg.frame_number < mNextReprocessShutterFrameNumber) {
                        SET_ERR("Shutter notification out-of-order. Expected "
 "notification for frame %d, got frame %d",
                                mNextReprocessShutterFrameNumber, msg.frame_number);
 return;
 }
                    mNextReprocessShutterFrameNumber = msg.frame_number + 1;
 } else {
 if (msg.frame_number < mNextShutterFrameNumber) {
                        SET_ERR("Shutter notification out-of-order. Expected "
 "notification for frame %d, got frame %d",
                                mNextShutterFrameNumber, msg.frame_number);
 return;
 }
                    mNextShutterFrameNumber = msg.frame_number + 1;
 }
 }

            ALOGVV("Camera %d: %s: Shutter fired for frame %d (id %d) at %" PRId64,
                    mId, __FUNCTION__,
                    msg.frame_number, r.resultExtras.requestId, msg.timestamp);
 if (listener != NULL) {
                listener->notifyShutter(r.resultExtras, msg.timestamp);
 }

            r.shutterTimestamp = msg.timestamp;

            sendCaptureResult(r.pendingMetadata, r.resultExtras,
                r.partialResult.collectedResult, msg.frame_number,
                r.hasInputBuffer, r.aeTriggerCancelOverride);
            returnOutputBuffers(r.pendingOutputBuffers.array(),
                r.pendingOutputBuffers.size(), r.shutterTimestamp);
            r.pendingOutputBuffers.clear();

            removeInFlightRequestIfReadyLocked(idx);
 }
 }
 if (idx < 0) {
        SET_ERR("Shutter notification for non-existent frame number %d",
                msg.frame_number);
 }
}
