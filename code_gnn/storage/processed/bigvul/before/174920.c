status_t Camera2Client::startPreviewL(Parameters &params, bool restart) {
    ATRACE_CALL();
 status_t res;

    ALOGV("%s: state == %d, restart = %d", __FUNCTION__, params.state, restart);

 if ( (params.state == Parameters::PREVIEW ||
                    params.state == Parameters::RECORD ||
                    params.state == Parameters::VIDEO_SNAPSHOT)
 && !restart) {
        ALOGI("%s: Camera %d: Preview already active, ignoring restart",
                __FUNCTION__, mCameraId);
 return OK;
 }
 if (params.state > Parameters::PREVIEW && !restart) {
        ALOGE("%s: Can't start preview in state %s",
                __FUNCTION__,
 Parameters::getStateName(params.state));
 return INVALID_OPERATION;
 }

 if (!mStreamingProcessor->haveValidPreviewWindow()) {
        params.state = Parameters::WAITING_FOR_PREVIEW_WINDOW;
 return OK;
 }
    params.state = Parameters::STOPPED;
 int lastPreviewStreamId = mStreamingProcessor->getPreviewStreamId();

    res = mStreamingProcessor->updatePreviewStream(params);
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to update preview stream: %s (%d)",
                __FUNCTION__, mCameraId, strerror(-res), res);
 return res;
 }

 bool previewStreamChanged = mStreamingProcessor->getPreviewStreamId() != lastPreviewStreamId;

    res = updateProcessorStream(mJpegProcessor, params);
 if (res != OK) {
        ALOGE("%s: Camera %d: Can't pre-configure still image "
 "stream: %s (%d)",
                __FUNCTION__, mCameraId, strerror(-res), res);
 return res;
 }

 Vector<int32_t> outputStreams;
 bool callbacksEnabled = (params.previewCallbackFlags &
            CAMERA_FRAME_CALLBACK_FLAG_ENABLE_MASK) ||
            params.previewCallbackSurface;

 if (callbacksEnabled) {
 if (mStreamingProcessor->getRecordingStreamId() != NO_STREAM) {
            ALOGV("%s: Camera %d: Clearing out recording stream before "
 "creating callback stream", __FUNCTION__, mCameraId);
            res = mStreamingProcessor->stopStream();
 if (res != OK) {
                ALOGE("%s: Camera %d: Can't stop streaming to delete "
 "recording stream", __FUNCTION__, mCameraId);
 return res;
 }
            res = mStreamingProcessor->deleteRecordingStream();
 if (res != OK) {
                ALOGE("%s: Camera %d: Unable to delete recording stream before "
 "enabling callbacks: %s (%d)", __FUNCTION__, mCameraId,
                        strerror(-res), res);
 return res;
 }
 }

        res = mCallbackProcessor->updateStream(params);
 if (res != OK) {
            ALOGE("%s: Camera %d: Unable to update callback stream: %s (%d)",
                    __FUNCTION__, mCameraId, strerror(-res), res);
 return res;
 }
        outputStreams.push(getCallbackStreamId());
 } else if (previewStreamChanged && mCallbackProcessor->getStreamId() != NO_STREAM) {
  
        ALOGV("%s: Camera %d: Delete unused preview callback stream.",  __FUNCTION__, mCameraId);
        res = mCallbackProcessor->deleteStream();
 if (res != OK) {
            ALOGE("%s: Camera %d: Unable to delete callback stream %s (%d)",
                    __FUNCTION__, mCameraId, strerror(-res), res);
 return res;
 }
 }
 if (params.zslMode && !params.recordingHint) {
        res = updateProcessorStream(mZslProcessor, params);
 if (res != OK) {
            ALOGE("%s: Camera %d: Unable to update ZSL stream: %s (%d)",
                    __FUNCTION__, mCameraId, strerror(-res), res);
 return res;
 }
        outputStreams.push(getZslStreamId());
 }

    outputStreams.push(getPreviewStreamId());

 if (!params.recordingHint) {
 if (!restart) {
            res = mStreamingProcessor->updatePreviewRequest(params);
 if (res != OK) {
                ALOGE("%s: Camera %d: Can't set up preview request: "
 "%s (%d)", __FUNCTION__, mCameraId,
                        strerror(-res), res);
 return res;
 }
 }
        res = mStreamingProcessor->startStream(StreamingProcessor::PREVIEW,
                outputStreams);
 } else {
 if (!restart) {
            res = mStreamingProcessor->updateRecordingRequest(params);
 if (res != OK) {
                ALOGE("%s: Camera %d: Can't set up preview request with "
 "record hint: %s (%d)", __FUNCTION__, mCameraId,
                        strerror(-res), res);
 return res;
 }
 }
        res = mStreamingProcessor->startStream(StreamingProcessor::RECORD,
                outputStreams);
 }
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to start streaming preview: %s (%d)",
                __FUNCTION__, mCameraId, strerror(-res), res);
 return res;
 }

    params.state = Parameters::PREVIEW;
 return OK;
}
