status_t Camera2Client::startRecordingL(Parameters &params, bool restart) {
 status_t res;

    ALOGV("%s: state == %d, restart = %d", __FUNCTION__, params.state, restart);

 switch (params.state) {
 case Parameters::STOPPED:
            res = startPreviewL(params, false);
 if (res != OK) return res;
 break;
 case Parameters::PREVIEW:
 break;
 case Parameters::RECORD:
 case Parameters::VIDEO_SNAPSHOT:
 if (!restart) return OK;
 break;
 default:
            ALOGE("%s: Camera %d: Can't start recording in state %s",
                    __FUNCTION__, mCameraId,
 Parameters::getStateName(params.state));
 return INVALID_OPERATION;
 };

 if (!params.storeMetadataInBuffers) {
        ALOGE("%s: Camera %d: Recording only supported in metadata mode, but "
 "non-metadata recording mode requested!", __FUNCTION__,
                mCameraId);
 return INVALID_OPERATION;
 }

 if (!restart) {
        mCameraService->playSound(CameraService::SOUND_RECORDING);
        mStreamingProcessor->updateRecordingRequest(params);
 if (res != OK) {
            ALOGE("%s: Camera %d: Unable to update recording request: %s (%d)",
                    __FUNCTION__, mCameraId, strerror(-res), res);
 return res;
 }
 }

 if (mCallbackProcessor->getStreamId() != NO_STREAM) {
        ALOGV("%s: Camera %d: Clearing out callback stream before "
 "creating recording stream", __FUNCTION__, mCameraId);
        res = mStreamingProcessor->stopStream();
 if (res != OK) {
            ALOGE("%s: Camera %d: Can't stop streaming to delete callback stream",
                    __FUNCTION__, mCameraId);
 return res;
 }
        res = mCallbackProcessor->deleteStream();
 if (res != OK) {
            ALOGE("%s: Camera %d: Unable to delete callback stream before "
 "record: %s (%d)", __FUNCTION__, mCameraId,
                    strerror(-res), res);
 return res;
 }
 }
    params.previewCallbackFlags = 0;

    res = updateProcessorStream<
 StreamingProcessor,
 &StreamingProcessor::updateRecordingStream>(mStreamingProcessor,
                                                        params);
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to update recording stream: %s (%d)",
                __FUNCTION__, mCameraId, strerror(-res), res);
 return res;
 }

 Vector<int32_t> outputStreams;
    outputStreams.push(getPreviewStreamId());
    outputStreams.push(getRecordingStreamId());

    res = mStreamingProcessor->startStream(StreamingProcessor::RECORD,
            outputStreams);
 if (res != OK) {
        ALOGE("%s: Camera %d: Unable to start recording stream: %s (%d)",
                __FUNCTION__, mCameraId, strerror(-res), res);
 return res;
 }

 if (params.state < Parameters::RECORD) {
        params.state = Parameters::RECORD;
 }

 return OK;
}
