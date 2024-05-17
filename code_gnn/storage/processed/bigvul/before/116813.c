  ~ReplaceContentClientRenderer() {
    content::GetContentClient()->set_renderer(saved_renderer_);
  }
