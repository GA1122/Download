void Camera3Device::processCaptureResult(const camera3_capture_result *result) {
    ATRACE_CALL();

 status_t res;

 uint32_t frameNumber = result->frame_number;
 if (result->result == NULL && result->num_output_buffers == 0 &&
            result->input_buffer == NULL) {
        SET_ERR("No result data provided by HAL for frame %d",
                frameNumber);
 return;
 }

 if (!mUsePartialResult &&
            mDeviceVersion >= CAMERA_DEVICE_API_VERSION_3_2 &&
            result->result != NULL &&
            result->partial_result != 1) {
        SET_ERR("Result is malformed for frame %d: partial_result %u must be 1"
 " if partial result is not supported",
                frameNumber, result->partial_result);
 return;
 }

 bool isPartialResult = false;
 CameraMetadata collectedPartialResult;
 CaptureResultExtras resultExtras;
 bool hasInputBufferInRequest = false;

 nsecs_t shutterTimestamp = 0;

 {
 Mutex::Autolock l(mInFlightLock);
 ssize_t idx = mInFlightMap.indexOfKey(frameNumber);
 if (idx == NAME_NOT_FOUND) {
            SET_ERR("Unknown frame number for capture result: %d",
                    frameNumber);
 return;
 }
 InFlightRequest &request = mInFlightMap.editValueAt(idx);
        ALOGVV("%s: got InFlightRequest requestId = %" PRId32
 ", frameNumber = %" PRId64 ", burstId = %" PRId32
 ", partialResultCount = %d",
                __FUNCTION__, request.resultExtras.requestId,
                request.resultExtras.frameNumber, request.resultExtras.burstId,
                result->partial_result);
 if (result->partial_result != 0)
            request.resultExtras.partialResultCount = result->partial_result;

 if (mUsePartialResult && result->result != NULL) {
 if (mDeviceVersion >= CAMERA_DEVICE_API_VERSION_3_2) {
 if (result->partial_result > mNumPartialResults || result->partial_result < 1) {
                    SET_ERR("Result is malformed for frame %d: partial_result %u must be  in"
 " the range of [1, %d] when metadata is included in the result",
                            frameNumber, result->partial_result, mNumPartialResults);
 return;
 }
                isPartialResult = (result->partial_result < mNumPartialResults);
 if (isPartialResult) {
                    request.partialResult.collectedResult.append(result->result);
 }
 } else {
 camera_metadata_ro_entry_t partialResultEntry;
                res = find_camera_metadata_ro_entry(result->result,
                        ANDROID_QUIRKS_PARTIAL_RESULT, &partialResultEntry);
 if (res != NAME_NOT_FOUND &&
                        partialResultEntry.count > 0 &&
                        partialResultEntry.data.u8[0] ==
                        ANDROID_QUIRKS_PARTIAL_RESULT_PARTIAL) {
                    isPartialResult = true;
                    request.partialResult.collectedResult.append(
                        result->result);
                    request.partialResult.collectedResult.erase(
                        ANDROID_QUIRKS_PARTIAL_RESULT);
 }
 }

 if (isPartialResult) {
 if (!request.partialResult.haveSent3A) {
                    request.partialResult.haveSent3A =
                            processPartial3AResult(frameNumber,
                                    request.partialResult.collectedResult,
                                    request.resultExtras);
 }
 }
 }

        shutterTimestamp = request.shutterTimestamp;
        hasInputBufferInRequest = request.hasInputBuffer;

 if (result->result != NULL && !isPartialResult) {
 if (request.haveResultMetadata) {
                SET_ERR("Called multiple times with metadata for frame %d",
                        frameNumber);
 return;
 }
 if (mUsePartialResult &&
 !request.partialResult.collectedResult.isEmpty()) {
                collectedPartialResult.acquire(
                    request.partialResult.collectedResult);
 }
            request.haveResultMetadata = true;
 }

 uint32_t numBuffersReturned = result->num_output_buffers;
 if (result->input_buffer != NULL) {
 if (hasInputBufferInRequest) {
                numBuffersReturned += 1;
 } else {
                ALOGW("%s: Input buffer should be NULL if there is no input"
 " buffer sent in the request",
                        __FUNCTION__);
 }
 }
        request.numBuffersLeft -= numBuffersReturned;
 if (request.numBuffersLeft < 0) {
            SET_ERR("Too many buffers returned for frame %d",
                    frameNumber);
 return;
 }

 camera_metadata_ro_entry_t entry;
        res = find_camera_metadata_ro_entry(result->result,
                ANDROID_SENSOR_TIMESTAMP, &entry);
 if (res == OK && entry.count == 1) {
            request.sensorTimestamp = entry.data.i64[0];
 }

 if (shutterTimestamp == 0) {
            request.pendingOutputBuffers.appendArray(result->output_buffers,
                result->num_output_buffers);
 } else {
            returnOutputBuffers(result->output_buffers,
                result->num_output_buffers, shutterTimestamp);
 }

 if (result->result != NULL && !isPartialResult) {
 if (shutterTimestamp == 0) {
                request.pendingMetadata = result->result;
                request.partialResult.collectedResult = collectedPartialResult;
 } else {
 CameraMetadata metadata;
                metadata = result->result;
                sendCaptureResult(metadata, request.resultExtras,
                    collectedPartialResult, frameNumber, hasInputBufferInRequest,
                    request.aeTriggerCancelOverride);
 }
 }

        removeInFlightRequestIfReadyLocked(idx);
 }  

 if (result->input_buffer != NULL) {
 if (hasInputBufferInRequest) {
 Camera3Stream *stream =
 Camera3Stream::cast(result->input_buffer->stream);
            res = stream->returnInputBuffer(*(result->input_buffer));
 if (res != OK) {
                ALOGE("%s: RequestThread: Can't return input buffer for frame %d to"
 "  its stream:%s (%d)",  __FUNCTION__,
                      frameNumber, strerror(-res), res);
 }
 } else {
            ALOGW("%s: Input buffer should be NULL if there is no input"
 " buffer sent in the request, skipping input buffer return.",
                    __FUNCTION__);
 }
 }
}
