static void DrawGLFunction(long view_context,
                           AwDrawGLInfo* draw_info,
                           void* spare) {
  reinterpret_cast<android_webview::SharedRendererState*>(view_context)
      ->DrawGL(draw_info);
}
