        querySurfaceMediaSourceFromMediaServer()
{
 Mutex::Autolock _l(mLock);
    mSurfaceMediaSource =
            mMediaRecorder->querySurfaceMediaSource();
 if (mSurfaceMediaSource == NULL) {
        ALOGE("SurfaceMediaSource could not be initialized!");
 }
 return mSurfaceMediaSource;
}
