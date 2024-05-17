status_t Camera2Client::commandStartFaceDetectionL(int  ) {
    ALOGV("%s: Camera %d: Starting face detection",
          __FUNCTION__, mCameraId);
 status_t res;
 SharedParameters::Lock l(mParameters);
 switch (l.mParameters.state) {
 case Parameters::DISCONNECTED:
 case Parameters::STOPPED:
 case Parameters::WAITING_FOR_PREVIEW_WINDOW:
 case Parameters::STILL_CAPTURE:
            ALOGE("%s: Camera %d: Cannot start face detection without preview active",
                    __FUNCTION__, mCameraId);
 return INVALID_OPERATION;
 case Parameters::PREVIEW:
 case Parameters::RECORD:
 case Parameters::VIDEO_SNAPSHOT:
 break;
 }
 if (l.mParameters.fastInfo.bestFaceDetectMode ==
            ANDROID_STATISTICS_FACE_DETECT_MODE_OFF) {
        ALOGE("%s: Camera %d: Face detection not supported",
                __FUNCTION__, mCameraId);
 return BAD_VALUE;
 }
 if (l.mParameters.enableFaceDetect) return OK;

    l.mParameters.enableFaceDetect = true;

    res = updateRequests(l.mParameters);

 return res;
}
