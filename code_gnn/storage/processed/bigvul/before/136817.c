Navigator* LocalDOMWindow::navigator() const {
  if (!navigator_)
    navigator_ = Navigator::Create(GetFrame());
  return navigator_.Get();
}
