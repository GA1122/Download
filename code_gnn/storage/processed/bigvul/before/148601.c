  ~AXTreeSnapshotCombiner() {
    combiner_.Combine();
    callback_.Run(combiner_.combined());
  }
