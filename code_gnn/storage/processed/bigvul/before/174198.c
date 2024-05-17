void Camera3Device::RequestThread::cleanUpFailedRequests(bool sendRequestError) {
 if (mNextRequests.empty()) {
 return;
 }

 for (auto& nextRequest : mNextRequests) {
 if (nextRequest.submitted) {
 continue;
 }

        sp<CaptureRequest> captureRequest = nextRequest.captureRequest;
 camera3_capture_request_t* halRequest = &nextRequest.halRequest;
 Vector<camera3_stream_buffer_t>* outputBuffers = &nextRequest.outputBuffers;

 if (halRequest->settings != NULL) {
            captureRequest->mSettings.unlock(halRequest->settings);
 }

 if (captureRequest->mInputStream != NULL) {
            captureRequest->mInputBuffer.status = CAMERA3_BUFFER_STATUS_ERROR;
            captureRequest->mInputStream->returnInputBuffer(captureRequest->mInputBuffer);
 }

 for (size_t i = 0; i < halRequest->num_output_buffers; i++) {
            outputBuffers->editItemAt(i).status = CAMERA3_BUFFER_STATUS_ERROR;
            captureRequest->mOutputStreams.editItemAt(i)->returnBuffer((*outputBuffers)[i], 0);
 }

 if (sendRequestError) {
 Mutex::Autolock l(mRequestLock);
 if (mListener != NULL) {
                mListener->notifyError(
 ICameraDeviceCallbacks::ERROR_CAMERA_REQUEST,
                        captureRequest->mResultExtras);
 }
 }
 }

 Mutex::Autolock l(mRequestLock);
    mNextRequests.clear();
}
