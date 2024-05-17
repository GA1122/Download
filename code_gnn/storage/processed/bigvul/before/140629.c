void OutOfProcessInstance::LoadPreviewUrl(const std::string& url) {
  LoadUrlInternal(url, &embed_preview_loader_,
                  &OutOfProcessInstance::DidOpenPreview);
}
