void AeroPeekWindow::OnClose() {
  Destroy();

  if (delegate_)
    delegate_->CloseTab(tab_id_);
}
