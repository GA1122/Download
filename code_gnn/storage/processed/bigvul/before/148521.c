  void ReceiveSnapshot(bool is_root, const ui::AXTreeUpdate& snapshot) {
    combiner_.AddTree(snapshot, is_root);
  }
