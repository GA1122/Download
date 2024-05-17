LayerTreeHostImpl::~LayerTreeHostImpl() {
  DCHECK(task_runner_provider_->IsImplThread());
  TRACE_EVENT0("cc", "LayerTreeHostImpl::~LayerTreeHostImpl()");
  TRACE_EVENT_OBJECT_DELETED_WITH_ID(TRACE_DISABLED_BY_DEFAULT("cc.debug"),
                                     "cc::LayerTreeHostImpl", id_);

  DCHECK(!compositor_frame_sink_);

  DCHECK(!resource_provider_);
  DCHECK(!resource_pool_);
  DCHECK(!single_thread_synchronous_task_graph_runner_);
  DCHECK(!image_decode_cache_);

  if (input_handler_client_) {
    input_handler_client_->WillShutdown();
    input_handler_client_ = NULL;
  }
  if (scroll_elasticity_helper_)
    scroll_elasticity_helper_.reset();

  if (recycle_tree_)
    recycle_tree_->Shutdown();
  if (pending_tree_)
    pending_tree_->Shutdown();
  active_tree_->Shutdown();
  recycle_tree_ = nullptr;
  pending_tree_ = nullptr;
  active_tree_ = nullptr;

  mutator_host_->ClearMutators();
  mutator_host_->SetMutatorHostClient(nullptr);
}
