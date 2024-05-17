status_t ACodec::configureOutputBuffersFromNativeWindow(
        OMX_U32 *bufferCount, OMX_U32 *bufferSize,
        OMX_U32 *minUndequeuedBuffers) {
    OMX_PARAM_PORTDEFINITIONTYPE def;
 InitOMXParams(&def);
    def.nPortIndex = kPortIndexOutput;

 status_t err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err == OK) {
        err = setupNativeWindowSizeFormatAndUsage(mNativeWindow.get(), &mNativeWindowUsageBits);
 }
 if (err != OK) {
        mNativeWindowUsageBits = 0;
 return err;
 }

 if (mTunneled) {
        ALOGV("Tunneled Playback: skipping native window buffer allocation.");
        def.nBufferCountActual = 0;
        err = mOMX->setParameter(
                mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 *minUndequeuedBuffers = 0;
 *bufferCount = 0;
 *bufferSize = 0;
 return err;
 }

 *minUndequeuedBuffers = 0;
    err = mNativeWindow->query(
            mNativeWindow.get(), NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS,
 (int *)minUndequeuedBuffers);

 if (err != 0) {
        ALOGE("NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS query failed: %s (%d)",
                strerror(-err), -err);
 return err;
 }


 for (OMX_U32 extraBuffers = 2 + 1;  ; extraBuffers--) {
        OMX_U32 newBufferCount =
            def.nBufferCountMin + *minUndequeuedBuffers + extraBuffers;
        def.nBufferCountActual = newBufferCount;
        err = mOMX->setParameter(
                mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err == OK) {
 *minUndequeuedBuffers += extraBuffers;
 break;
 }

        ALOGW("[%s] setting nBufferCountActual to %u failed: %d",
                mComponentName.c_str(), newBufferCount, err);
  
 if (extraBuffers == 0) {
 return err;
 }
 }

    err = native_window_set_buffer_count(
            mNativeWindow.get(), def.nBufferCountActual);

 if (err != 0) {
        ALOGE("native_window_set_buffer_count failed: %s (%d)", strerror(-err),
 -err);
 return err;
 }

 *bufferCount = def.nBufferCountActual;
 *bufferSize =  def.nBufferSize;
 return err;
}
