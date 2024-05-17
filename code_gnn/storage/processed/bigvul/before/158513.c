Node* WebLocalFrameImpl::ContextMenuNodeInner() const {
  if (!ViewImpl() || !ViewImpl()->GetPage())
    return nullptr;
  return ViewImpl()
      ->GetPage()
      ->GetContextMenuController()
      .ContextMenuNodeForFrame(GetFrame());
}
