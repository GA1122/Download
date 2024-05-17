void PictureLayerImpl::UpdateTilePriorities(
    const Occlusion& occlusion_in_content_space) {
  DCHECK_IMPLIES(raster_source_->IsSolidColor(), tilings_->num_tilings() == 0);

  double current_frame_time_in_seconds =
      (layer_tree_impl()->CurrentBeginFrameArgs().frame_time -
       base::TimeTicks()).InSecondsF();
  gfx::Rect viewport_rect_in_layer_space =
      GetViewportForTilePriorityInContentSpace();

  bool can_require_tiles_for_activation =
      !only_used_low_res_last_append_quads_ || RequiresHighResToDraw() ||
      !layer_tree_impl()->SmoothnessTakesPriority();

  bool updated = tilings_->UpdateTilePriorities(
      viewport_rect_in_layer_space, ideal_contents_scale_,
      current_frame_time_in_seconds, occlusion_in_content_space,
      can_require_tiles_for_activation);

  if (updated)
    layer_tree_impl()->DidModifyTilePriorities();
}
