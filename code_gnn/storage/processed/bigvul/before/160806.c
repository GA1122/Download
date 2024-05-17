void RenderViewImpl::OnPageWasHidden() {
#if defined(OS_ANDROID)
  SuspendVideoCaptureDevices(true);
#if BUILDFLAG(ENABLE_WEBRTC)
  if (speech_recognition_dispatcher_)
    speech_recognition_dispatcher_->AbortAllRecognitions();
#endif
#endif

  if (webview()) {
    blink::mojom::PageVisibilityState visibilityState =
        GetMainRenderFrame() ? GetMainRenderFrame()->VisibilityState()
                             : blink::mojom::PageVisibilityState::kHidden;
    webview()->SetVisibilityState(visibilityState, false);
  }
}
