void RenderViewImpl::OnPageWasHidden() {
#if defined(OS_ANDROID)
  SuspendVideoCaptureDevices(true);
#endif

  if (webview()) {
    blink::mojom::PageVisibilityState visibilityState =
        GetMainRenderFrame() ? GetMainRenderFrame()->VisibilityState()
                             : blink::mojom::PageVisibilityState::kHidden;
    webview()->SetVisibilityState(visibilityState, false);
  }
}
