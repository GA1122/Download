void WebGLRenderingContextBase::SetupFlags() {
  DCHECK(GetDrawingBuffer());
  if (canvas()) {
    if (Page* p = canvas()->GetDocument().GetPage()) {
      synthesized_errors_to_console_ =
          p->GetSettings().GetWebGLErrorsToConsoleEnabled();
    }
  }

  is_depth_stencil_supported_ =
      ExtensionsUtil()->IsExtensionEnabled("GL_OES_packed_depth_stencil");
}
