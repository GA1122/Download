void LayerTreeHost::PaintLayerContents(
    const RenderSurfaceLayerList& render_surface_layer_list,
    ResourceUpdateQueue* queue,
    bool* did_paint_content,
    bool* need_more_updates) {
  OcclusionTracker<Layer> occlusion_tracker(
      root_layer_->render_surface()->content_rect());
  occlusion_tracker.set_minimum_tracking_size(
      settings_.minimum_occlusion_tracking_size);

  PrioritizeTextures(render_surface_layer_list);

  in_paint_layer_contents_ = true;

  typedef LayerIterator<Layer> LayerIteratorType;
  LayerIteratorType end = LayerIteratorType::End(&render_surface_layer_list);
  for (LayerIteratorType it =
           LayerIteratorType::Begin(&render_surface_layer_list);
       it != end;
       ++it) {
    occlusion_tracker.EnterLayer(it);

    if (it.represents_target_render_surface()) {
      PaintMasksForRenderSurface(
          *it, queue, did_paint_content, need_more_updates);
    } else if (it.represents_itself() && it->DrawsContent()) {
      DCHECK(!it->paint_properties().bounds.IsEmpty());
      *did_paint_content |= it->Update(queue, &occlusion_tracker);
      *need_more_updates |= it->NeedMoreUpdates();
    }

    occlusion_tracker.LeaveLayer(it);
  }

  in_paint_layer_contents_ = false;
}
