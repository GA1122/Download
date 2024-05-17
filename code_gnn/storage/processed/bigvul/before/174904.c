status_t Camera2Client::initialize(camera_module_t *module)
{
    ATRACE_CALL();
    ALOGV("%s: Initializing client for camera %d", __FUNCTION__, mCameraId);
 status_t res;

    res = Camera2ClientBase::initialize(module);
 if (res != OK) {
 return res;
 }

 {
 SharedParameters::Lock l(mParameters);

        res = l.mParameters.initialize(&(mDevice->info()));
 if (res != OK) {
            ALOGE("%s: Camera %d: unable to build defaults: %s (%d)",
                    __FUNCTION__, mCameraId, strerror(-res), res);
 return NO_INIT;
 }
 }

 String8 threadName;

    mStreamingProcessor = new StreamingProcessor(this);
    threadName = String8::format("C2-%d-StreamProc",
            mCameraId);
    mStreamingProcessor->run(threadName.string());

    mFrameProcessor = new FrameProcessor(mDevice, this);
    threadName = String8::format("C2-%d-FrameProc",
            mCameraId);
    mFrameProcessor->run(threadName.string());

    mCaptureSequencer = new CaptureSequencer(this);
    threadName = String8::format("C2-%d-CaptureSeq",
            mCameraId);
    mCaptureSequencer->run(threadName.string());

    mJpegProcessor = new JpegProcessor(this, mCaptureSequencer);
    threadName = String8::format("C2-%d-JpegProc",
            mCameraId);
    mJpegProcessor->run(threadName.string());

 switch (mDeviceVersion) {
 case CAMERA_DEVICE_API_VERSION_2_0: {
            sp<ZslProcessor> zslProc =
 new ZslProcessor(this, mCaptureSequencer);
            mZslProcessor = zslProc;
            mZslProcessorThread = zslProc;
 break;
 }
 case CAMERA_DEVICE_API_VERSION_3_0:{
            sp<ZslProcessor3> zslProc =
 new ZslProcessor3(this, mCaptureSequencer);
            mZslProcessor = zslProc;
            mZslProcessorThread = zslProc;
 break;
 }
 default:
 break;
 }
    threadName = String8::format("C2-%d-ZslProc",
            mCameraId);
    mZslProcessorThread->run(threadName.string());

    mCallbackProcessor = new CallbackProcessor(this);
    threadName = String8::format("C2-%d-CallbkProc",
            mCameraId);
    mCallbackProcessor->run(threadName.string());

 if (gLogLevel >= 1) {
 SharedParameters::Lock l(mParameters);
        ALOGD("%s: Default parameters converted from camera %d:", __FUNCTION__,
              mCameraId);
        ALOGD("%s", l.mParameters.paramsFlattened.string());
 }

 return OK;
}
