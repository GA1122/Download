status_t ACodec::configureTunneledVideoPlayback(
 int32_t audioHwSync, const sp<ANativeWindow> &nativeWindow) {
 native_handle_t* sidebandHandle;

 status_t err = mOMX->configureVideoTunnelMode(
            mNode, kPortIndexOutput, OMX_TRUE, audioHwSync, &sidebandHandle);
 if (err != OK) {
        ALOGE("configureVideoTunnelMode failed! (err %d).", err);
 return err;
 }

    err = native_window_set_sideband_stream(nativeWindow.get(), sidebandHandle);
 if (err != OK) {
        ALOGE("native_window_set_sideband_stream(%p) failed! (err %d).",
                sidebandHandle, err);
 return err;
 }

 return OK;
}
