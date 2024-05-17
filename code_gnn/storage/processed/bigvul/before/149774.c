void LayerTreeHost::ApplyScrollAndScale(ScrollAndScaleSet* info) {
  for (auto& swap_promise : info->swap_promises) {
    TRACE_EVENT_WITH_FLOW1("input,benchmark", "LatencyInfo.Flow",
                           TRACE_ID_DONT_MANGLE(swap_promise->TraceId()),
                           TRACE_EVENT_FLAG_FLOW_IN | TRACE_EVENT_FLAG_FLOW_OUT,
                           "step", "Main thread scroll update");
    swap_promise_manager_.QueueSwapPromise(std::move(swap_promise));
  }

  if (root_layer_) {
    for (size_t i = 0; i < info->scrolls.size(); ++i) {
      Layer* layer = LayerById(info->scrolls[i].layer_id);
      if (!layer)
        continue;
      layer->SetScrollOffsetFromImplSide(gfx::ScrollOffsetWithDelta(
          layer->scroll_offset(), info->scrolls[i].scroll_delta));
      SetNeedsUpdateLayers();
    }
    for (size_t i = 0; i < info->scrollbars.size(); ++i) {
      Layer* layer = LayerById(info->scrollbars[i].layer_id);
      if (!layer)
        continue;
      layer->SetScrollbarsHiddenFromImplSide(info->scrollbars[i].hidden);
    }
  }

  ApplyViewportDeltas(info);
}
