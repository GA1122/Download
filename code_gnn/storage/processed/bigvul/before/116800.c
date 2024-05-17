  explicit ReplaceContentClientRenderer(
      content::ContentRendererClient* new_renderer) {
    saved_renderer_ = content::GetContentClient()->renderer();
    content::GetContentClient()->set_renderer(new_renderer);
  }
