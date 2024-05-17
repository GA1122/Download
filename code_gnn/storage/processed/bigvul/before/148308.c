  AXTreeSnapshotCallback AddFrame(bool is_root) {
    return base::Bind(&AXTreeSnapshotCombiner::ReceiveSnapshot,
                      this,
                      is_root);
  }
