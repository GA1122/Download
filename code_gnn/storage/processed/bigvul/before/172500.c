CameraSource::CameraSource(
 const sp<ICamera>& camera,
 const sp<ICameraRecordingProxy>& proxy,
 int32_t cameraId,
 const String16& clientName,
 uid_t clientUid,
 Size videoSize,
 int32_t frameRate,
 const sp<IGraphicBufferProducer>& surface,
 bool storeMetaDataInVideoBuffers)
 : mCameraFlags(0),
      mNumInputBuffers(0),
      mVideoFrameRate(-1),
      mCamera(0),
      mSurface(surface),
      mNumFramesReceived(0),
      mLastFrameTimestampUs(0),
      mStarted(false),
      mNumFramesEncoded(0),
      mTimeBetweenFrameCaptureUs(0),
      mFirstFrameTimeUs(0),
      mNumFramesDropped(0),
      mNumGlitches(0),
      mGlitchDurationThresholdUs(200000),
      mCollectStats(false) {
    mVideoSize.width  = -1;
    mVideoSize.height = -1;

    mInitCheck = init(camera, proxy, cameraId,
                    clientName, clientUid,
                    videoSize, frameRate,
                    storeMetaDataInVideoBuffers);
 if (mInitCheck != OK) releaseCamera();
}
