void LayerTreeHostImpl::BeginMainFrameAborted(
    CommitEarlyOutReason reason,
    std::vector<std::unique_ptr<SwapPromise>> swap_promises) {
  if (CommitEarlyOutHandledCommit(reason)) {
    active_tree_->ApplySentScrollAndScaleDeltasFromAbortedCommit();
    if (pending_tree_) {
      pending_tree_->AppendSwapPromises(std::move(swap_promises));
    } else {
      for (const auto& swap_promise : swap_promises)
        swap_promise->DidNotSwap(SwapPromise::COMMIT_NO_UPDATE);
    }
  }
}
