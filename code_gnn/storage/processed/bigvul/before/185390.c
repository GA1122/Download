  void CreateDistillerJavaScriptService(
      content::RenderFrameHost* render_frame_host,
    ExternalFeedbackReporter* feedback_reporter,
//     DistillerUIHandle* distiller_ui_handle,
      mojo::InterfaceRequest<DistillerJavaScriptService> request) {
  new DistillerJavaScriptServiceImpl(render_frame_host, feedback_reporter,
//   new DistillerJavaScriptServiceImpl(render_frame_host, distiller_ui_handle,
        request.Pass());
  }