void LayerTreeHostImpl::CleanUpTileManagerAndUIResources() {
  ClearUIResources();
  tile_manager_.FinishTasksAndCleanUp();
  resource_pool_ = nullptr;
  single_thread_synchronous_task_graph_runner_ = nullptr;
  image_decode_cache_ = nullptr;

  if (compositor_frame_sink_) {
    if (auto* compositor_context = compositor_frame_sink_->context_provider())
      compositor_context->ContextGL()->ShallowFlushCHROMIUM();
    if (auto* worker_context =
            compositor_frame_sink_->worker_context_provider()) {
      ContextProvider::ScopedContextLock hold(worker_context);
      worker_context->ContextGL()->ShallowFlushCHROMIUM();
    }
  }
}
