  void ChangeFrame() {
    switch (layer_tree_host()->SourceFrameNumber()) {
      case 1:
        layer_tree_host()->GetSwapPromiseManager()->QueueSwapPromise(
            base::MakeUnique<TestSwapPromise>(&swap_promise_result_));
        layer_tree_host()->SetNeedsUpdateLayers();
        break;
      case 2:
        break;
      default:
        NOTREACHED();
        break;
    }
  }
