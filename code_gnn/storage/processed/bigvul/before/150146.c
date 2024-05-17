  SimpleSwapPromiseMonitor(LayerTreeHost* layer_tree_host,
                           LayerTreeHostImpl* layer_tree_host_impl,
                           int* set_needs_commit_count,
                           int* set_needs_redraw_count)
      : SwapPromiseMonitor(
            (layer_tree_host ? layer_tree_host->GetSwapPromiseManager()
                             : nullptr),
            layer_tree_host_impl),
        set_needs_commit_count_(set_needs_commit_count) {}
