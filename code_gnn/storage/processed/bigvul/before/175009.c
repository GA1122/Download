status_t ProCamera2Client::initialize(camera_module_t *module)
{
    ATRACE_CALL();
 status_t res;

    res = Camera2ClientBase::initialize(module);
 if (res != OK) {
 return res;
 }

 String8 threadName;
    mFrameProcessor = new FrameProcessorBase(mDevice);
    threadName = String8::format("PC2-%d-FrameProc", mCameraId);
    mFrameProcessor->run(threadName.string());

    mFrameProcessor->registerListener(FRAME_PROCESSOR_LISTENER_MIN_ID,
                                      FRAME_PROCESSOR_LISTENER_MAX_ID,
  this);

 return OK;
}
