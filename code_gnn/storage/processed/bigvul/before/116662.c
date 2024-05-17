bool RenderViewImpl::SupportsAsynchronousSwapBuffers() {
  if (WebWidgetHandlesCompositorScheduling())
    return false;

  if (queried_for_swapbuffers_complete_callback_)
    return context_has_swapbuffers_complete_callback_;

  queried_for_swapbuffers_complete_callback_ = true;

  WebKit::WebGraphicsContext3D* context = webview()->graphicsContext3D();
  if (context && context->makeContextCurrent()) {
    std::string extensions(context->getRequestableExtensionsCHROMIUM().utf8());
    context_has_swapbuffers_complete_callback_ =
        extensions.find("GL_CHROMIUM_swapbuffers_complete_callback")
            != std::string::npos;
  }

  return context_has_swapbuffers_complete_callback_;
}
