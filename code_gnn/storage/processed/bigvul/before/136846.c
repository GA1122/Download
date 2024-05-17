StyleMedia* LocalDOMWindow::styleMedia() const {
  if (!media_)
    media_ = StyleMedia::Create(GetFrame());
  return media_.Get();
}
