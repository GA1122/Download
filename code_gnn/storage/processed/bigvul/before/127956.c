bool BrowserViewRenderer::RequestDrawGL(bool wait_for_completion) {
  return client_->RequestDrawGL(wait_for_completion);
}
