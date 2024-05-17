void LayerTreeHost::FinishCommitOnImplThread(LayerTreeHostImpl* host_impl) {
  DCHECK(proxy_->IsImplThread());

  bool new_impl_tree_has_no_evicted_resources = false;
  if (contents_texture_manager_) {
    new_impl_tree_has_no_evicted_resources =
        !contents_texture_manager_->LinkedEvictedBackingsExist();

    if (contents_texture_manager_->MaxMemoryLimitBytes() <
            host_impl->memory_allocation_limit_bytes() &&
        contents_texture_manager_->MaxMemoryLimitBytes() <
            contents_texture_manager_->MaxMemoryNeededBytes()) {
      host_impl->SetNeedsCommit();
    }

    host_impl->set_max_memory_needed_bytes(
        contents_texture_manager_->MaxMemoryNeededBytes());

    contents_texture_manager_->UpdateBackingsState(
        host_impl->resource_provider());
  }

  LayerTreeImpl* sync_tree;
  if (settings_.impl_side_painting) {
    DCHECK(!host_impl->pending_tree());
    host_impl->CreatePendingTree();
    sync_tree = host_impl->pending_tree();
    if (next_commit_forces_redraw_)
      sync_tree->ForceRedrawNextActivation();
  } else {
    if (next_commit_forces_redraw_)
      host_impl->SetFullRootLayerDamage();
    contents_texture_manager_->ReduceMemory(host_impl->resource_provider());
    sync_tree = host_impl->active_tree();
  }

  next_commit_forces_redraw_ = false;

  sync_tree->set_source_frame_number(source_frame_number());

  if (needs_full_tree_sync_)
    sync_tree->SetRootLayer(TreeSynchronizer::SynchronizeTrees(
        root_layer(), sync_tree->DetachLayerTree(), sync_tree));
  {
    TRACE_EVENT0("cc", "LayerTreeHost::PushProperties");
    TreeSynchronizer::PushProperties(root_layer(), sync_tree->root_layer());
  }

  sync_tree->set_needs_full_tree_sync(needs_full_tree_sync_);
  needs_full_tree_sync_ = false;

  if (hud_layer_.get()) {
    LayerImpl* hud_impl = LayerTreeHostCommon::FindLayerInSubtree(
        sync_tree->root_layer(), hud_layer_->id());
    sync_tree->set_hud_layer(static_cast<HeadsUpDisplayLayerImpl*>(hud_impl));
  } else {
    sync_tree->set_hud_layer(NULL);
  }

  sync_tree->set_background_color(background_color_);
  sync_tree->set_has_transparent_background(has_transparent_background_);

  if (page_scale_layer_ && inner_viewport_scroll_layer_) {
    sync_tree->SetViewportLayersFromIds(
        page_scale_layer_->id(),
        inner_viewport_scroll_layer_->id(),
        outer_viewport_scroll_layer_ ? outer_viewport_scroll_layer_->id()
                                     : Layer::INVALID_ID);
  } else {
    sync_tree->ClearViewportLayers();
  }

  float page_scale_delta, sent_page_scale_delta;
  if (settings_.impl_side_painting) {
    DCHECK_EQ(1.f, sync_tree->sent_page_scale_delta());
    page_scale_delta = host_impl->active_tree()->page_scale_delta();
    sent_page_scale_delta = host_impl->active_tree()->sent_page_scale_delta();
  } else {
    page_scale_delta = sync_tree->page_scale_delta();
    sent_page_scale_delta = sync_tree->sent_page_scale_delta();
    sync_tree->set_sent_page_scale_delta(1.f);
  }

  sync_tree->SetPageScaleFactorAndLimits(page_scale_factor_,
                                         min_page_scale_factor_,
                                         max_page_scale_factor_);
  sync_tree->SetPageScaleDelta(page_scale_delta / sent_page_scale_delta);

  sync_tree->PassSwapPromises(&swap_promise_list_);

  host_impl->SetViewportSize(device_viewport_size_);
  host_impl->SetOverdrawBottomHeight(overdraw_bottom_height_);
  host_impl->SetDeviceScaleFactor(device_scale_factor_);
  host_impl->SetDebugState(debug_state_);
  if (pending_page_scale_animation_) {
    host_impl->StartPageScaleAnimation(
        pending_page_scale_animation_->target_offset,
        pending_page_scale_animation_->use_anchor,
        pending_page_scale_animation_->scale,
        pending_page_scale_animation_->duration);
    pending_page_scale_animation_.reset();
  }

  if (!ui_resource_request_queue_.empty()) {
    sync_tree->set_ui_resource_request_queue(ui_resource_request_queue_);
    ui_resource_request_queue_.clear();
    if (!settings_.impl_side_painting)
      sync_tree->ProcessUIResourceRequestQueue();
  }
  if (overhang_ui_resource_) {
    host_impl->SetOverhangUIResource(
        overhang_ui_resource_->id(),
        GetUIResourceSize(overhang_ui_resource_->id()));
  }

  DCHECK(!sync_tree->ViewportSizeInvalid());

  if (new_impl_tree_has_no_evicted_resources) {
    if (sync_tree->ContentsTexturesPurged())
      sync_tree->ResetContentsTexturesPurged();
  }

  if (!settings_.impl_side_painting) {
    sync_tree->DidBecomeActive();
    devtools_instrumentation::DidActivateLayerTree(id_, source_frame_number_);
  }

  micro_benchmark_controller_.ScheduleImplBenchmarks(host_impl);

  source_frame_number_++;
}
