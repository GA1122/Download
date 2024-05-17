void RenderViewImpl::OnPageWasShown() {
#if defined(OS_ANDROID)
  SuspendVideoCaptureDevices(false);
#endif

  if (webview()) {
    blink::mojom::PageVisibilityState visibilityState =
        GetMainRenderFrame() ? GetMainRenderFrame()->VisibilityState()
                             : blink::mojom::PageVisibilityState::kVisible;
    webview()->SetVisibilityState(visibilityState, false);
  }
}
