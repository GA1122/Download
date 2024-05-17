bool Document::ShouldInstallV8Extensions() const {
  return frame_->Client()->AllowScriptExtensions();
}
