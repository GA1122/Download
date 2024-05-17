scoped_ptr<LayerTreeHostImpl> LayerTreeHost::CreateLayerTreeHostImpl(
    LayerTreeHostImplClient* client) {
  DCHECK(proxy_->IsImplThread());
  scoped_ptr<LayerTreeHostImpl> host_impl =
      LayerTreeHostImpl::Create(settings_,
                                client,
                                proxy_.get(),
                                rendering_stats_instrumentation_.get(),
                                shared_bitmap_manager_,
                                id_);
  shared_bitmap_manager_ = NULL;
  if (settings_.calculate_top_controls_position &&
      host_impl->top_controls_manager()) {
    top_controls_manager_weak_ptr_ =
        host_impl->top_controls_manager()->AsWeakPtr();
  }
  input_handler_weak_ptr_ = host_impl->AsWeakPtr();
  return host_impl.Pass();
}
