void RenderViewImpl::SetEditCommandForNextKeyEvent(const std::string& name,
                                                   const std::string& value) {
  GetWidget()->SetEditCommandForNextKeyEvent(name, value);
}
