  void SetVisibleFalseAndQueueSwapPromise() {
    layer_tree_host()->SetVisible(false);
    std::unique_ptr<SwapPromise> swap_promise(
        new TestSwapPromise(&swap_promise_result_));
    layer_tree_host()->GetSwapPromiseManager()->QueueSwapPromise(
        std::move(swap_promise));
  }
