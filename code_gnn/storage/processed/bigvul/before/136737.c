void LocalDOMWindow::ClearDocument() {
  if (!document_)
    return;

  DCHECK(!document_->IsActive());

  ClearEventQueue();

  unused_preloads_timer_.Stop();
  document_->ClearDOMWindow();
  document_ = nullptr;
}
