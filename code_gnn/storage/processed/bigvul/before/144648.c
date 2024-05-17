void WebContentsImpl::RequestAXTreeSnapshot(AXTreeSnapshotCallback callback) {
  GetMainFrame()->RequestAXTreeSnapshot(callback);
}
