LayerTreeHost::OnCreateAndInitializeOutputSurfaceAttempted(bool success) {
  TRACE_EVENT1("cc",
               "LayerTreeHost::OnCreateAndInitializeOutputSurfaceAttempted",
               "success",
               success);

  DCHECK(output_surface_lost_);
  if (success) {
    output_surface_lost_ = false;

    if (!contents_texture_manager_ && !settings_.impl_side_painting) {
      contents_texture_manager_ =
          PrioritizedResourceManager::Create(proxy_.get());
      surface_memory_placeholder_ =
          contents_texture_manager_->CreateTexture(gfx::Size(), RGBA_8888);
    }

    if (root_layer()) {
      LayerTreeHostCommon::CallFunctionForSubtree(
          root_layer(),
          base::Bind(&LayerTreeHostOnOutputSurfaceCreatedCallback));
    }

    client_->DidInitializeOutputSurface(true);
    return CreateSucceeded;
  }


  client_->DidFailToInitializeOutputSurface();

  ++num_failed_recreate_attempts_;
  if (num_failed_recreate_attempts_ >= 5) {
    output_surface_can_be_initialized_ = false;
    client_->DidInitializeOutputSurface(false);
    return CreateFailedAndGaveUp;
  }

  return CreateFailedButTryAgain;
}
