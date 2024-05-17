OMX_ERRORTYPE SoftAVCEncoder::releaseEncoder() {
 if (!mStarted) {
 return OMX_ErrorNone;
 }

 PVAVCCleanUpEncoder(mHandle);
    releaseOutputBuffers();

    free(mInputFrameData);
    mInputFrameData = NULL;

    free(mSliceGroup);
    mSliceGroup = NULL;

 delete mEncParams;
    mEncParams = NULL;

 delete mHandle;
    mHandle = NULL;

    mStarted = false;

 return OMX_ErrorNone;
}
