void LayerTreeHostImpl::ActivateSyncTree() {
  if (pending_tree_) {
    TRACE_EVENT_ASYNC_END0("cc", "PendingTree:waiting", pending_tree_.get());

    DCHECK(pending_tree_duration_timer_);
    pending_tree_duration_timer_.reset();

    pending_tree_->ProcessUIResourceRequestQueue();

    if (pending_tree_->needs_full_tree_sync()) {
      TreeSynchronizer::SynchronizeTrees(pending_tree_.get(),
                                         active_tree_.get());
    }

    if (active_tree_->property_trees()->changed) {
      if (pending_tree_->property_trees()->sequence_number ==
          active_tree_->property_trees()->sequence_number)
        active_tree_->property_trees()->PushChangeTrackingTo(
            pending_tree_->property_trees());
      else
        active_tree_->MoveChangeTrackingToLayers();
    }
    active_tree_->property_trees()->PushOpacityIfNeeded(
        pending_tree_->property_trees());

    TreeSynchronizer::PushLayerProperties(pending_tree(), active_tree());
    pending_tree_->PushPropertiesTo(active_tree_.get());
    if (!pending_tree_->LayerListIsEmpty())
      pending_tree_->property_trees()->ResetAllChangeTracking();

    DCHECK(!recycle_tree_);
    pending_tree_.swap(recycle_tree_);

    ActivateAnimations();

    Mutate(CurrentBeginFrameArgs().frame_time);
  } else {
    active_tree_->ProcessUIResourceRequestQueue();
  }

  UpdateViewportContainerSizes();

  active_tree_->DidBecomeActive();
  client_->RenewTreePriority();
  if (!active_tree_->picture_layers().empty())
    DidModifyTilePriorities();

  tile_manager_.DidActivateSyncTree();
  client_->OnCanDrawStateChanged(CanDraw());
  client_->DidActivateSyncTree();
  if (!tree_activation_callback_.is_null())
    tree_activation_callback_.Run();

  std::unique_ptr<PendingPageScaleAnimation> pending_page_scale_animation =
      active_tree_->TakePendingPageScaleAnimation();
  if (pending_page_scale_animation) {
    StartPageScaleAnimation(pending_page_scale_animation->target_offset,
                            pending_page_scale_animation->use_anchor,
                            pending_page_scale_animation->scale,
                            pending_page_scale_animation->duration);
  }
  UpdateRootLayerStateForSynchronousInputHandler();
}
