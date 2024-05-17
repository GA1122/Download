MediaRecorder::~MediaRecorder()
{
    ALOGV("destructor");
 if (mMediaRecorder != NULL) {
        mMediaRecorder.clear();
 }

 if (mSurfaceMediaSource != NULL) {
        mSurfaceMediaSource.clear();
 }
}
