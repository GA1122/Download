status_t Camera2Client::commandSetVideoBufferCountL(size_t count) {
 if (recordingEnabledL()) {
        ALOGE("%s: Camera %d: Error setting video buffer count after "
 "recording was started", __FUNCTION__, mCameraId);
 return INVALID_OPERATION;
 }

 return mStreamingProcessor->setRecordingBufferCount(count);
}
