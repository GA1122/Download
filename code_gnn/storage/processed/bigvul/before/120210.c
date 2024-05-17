void LayerTreeHost::CommitComplete() {
  client_->DidCommit();
}
