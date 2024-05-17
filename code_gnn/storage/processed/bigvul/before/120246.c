void LayerTreeHost::QueueSwapPromise(scoped_ptr<SwapPromise> swap_promise) {
  DCHECK(swap_promise);
  if (swap_promise_list_.size() > kMaxQueuedSwapPromiseNumber)
    BreakSwapPromises(SwapPromise::SWAP_PROMISE_LIST_OVERFLOW);
  swap_promise_list_.push_back(swap_promise.Pass());
}
