  void ChangeFrame() {
    switch (layer_tree_host()->SourceFrameNumber()) {
      case 1:
        layer_->SetBounds(gfx::Size(10, 11));
        layer_tree_host()->GetSwapPromiseManager()->QueueSwapPromise(
            base::MakeUnique<TestSwapPromise>(&swap_promise_result_));
        break;
      case 2:
        break;
      default:
        NOTREACHED();
        break;
    }
  }
