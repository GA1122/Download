void LayerTreeHostImpl::CommitComplete() {
  TRACE_EVENT0("cc", "LayerTreeHostImpl::CommitComplete");

  UpdateSyncTreeAfterCommitOrImplSideInvalidation();
  micro_benchmark_controller_.DidCompleteCommit();
}
