  RenderMediaClientTest()
      : render_media_client_(RenderMediaClient::GetInstance()) {
    SetContentClient(&test_content_client_);
    SetRendererClientForTesting(&test_content_renderer_client_);
  }
