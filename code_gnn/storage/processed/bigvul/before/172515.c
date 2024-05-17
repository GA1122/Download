status_t CameraSource::initWithCameraAccess(
 const sp<ICamera>& camera,
 const sp<ICameraRecordingProxy>& proxy,
 int32_t cameraId,
 const String16& clientName,
 uid_t clientUid,
 Size videoSize,
 int32_t frameRate,
 bool storeMetaDataInVideoBuffers) {
    ALOGV("initWithCameraAccess");
 status_t err = OK;

 if ((err = isCameraAvailable(camera, proxy, cameraId,
            clientName, clientUid)) != OK) {
        ALOGE("Camera connection could not be established.");
 return err;
 }
 CameraParameters params(mCamera->getParameters());
 if ((err = isCameraColorFormatSupported(params)) != OK) {
 return err;
 }

 if ((err = configureCamera(&params,
                    videoSize.width, videoSize.height,
                    frameRate))) {
 return err;
 }

 CameraParameters newCameraParams(mCamera->getParameters());
 if ((err = checkVideoSize(newCameraParams,
                videoSize.width, videoSize.height)) != OK) {
 return err;
 }
 if ((err = checkFrameRate(newCameraParams, frameRate)) != OK) {
 return err;
 }

 if (mSurface != NULL) {
        CHECK_EQ((status_t)OK, mCamera->setPreviewTarget(mSurface));
 }

    mIsMetaDataStoredInVideoBuffers = false;
    mCamera->storeMetaDataInBuffers(false);
 if (storeMetaDataInVideoBuffers) {
 if (OK == mCamera->storeMetaDataInBuffers(true)) {
            mIsMetaDataStoredInVideoBuffers = true;
 }
 }

 int64_t glitchDurationUs = (1000000LL / mVideoFrameRate);
 if (glitchDurationUs > mGlitchDurationThresholdUs) {
        mGlitchDurationThresholdUs = glitchDurationUs;
 }

    mMeta = new MetaData;
    mMeta->setCString(kKeyMIMEType,  MEDIA_MIMETYPE_VIDEO_RAW);
    mMeta->setInt32(kKeyColorFormat, mColorFormat);
    mMeta->setInt32(kKeyWidth,       mVideoSize.width);
    mMeta->setInt32(kKeyHeight,      mVideoSize.height);
    mMeta->setInt32(kKeyStride,      mVideoSize.width);
    mMeta->setInt32(kKeySliceHeight, mVideoSize.height);
    mMeta->setInt32(kKeyFrameRate,   mVideoFrameRate);
 return OK;
}
