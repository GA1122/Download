void LayerTreeHost::PaintMasksForRenderSurface(Layer* render_surface_layer,
                                               ResourceUpdateQueue* queue,
                                               bool* did_paint_content,
                                               bool* need_more_updates) {

  Layer* mask_layer = render_surface_layer->mask_layer();
  if (mask_layer) {
    *did_paint_content |= mask_layer->Update(queue, NULL);
    *need_more_updates |= mask_layer->NeedMoreUpdates();
  }

  Layer* replica_mask_layer =
      render_surface_layer->replica_layer() ?
      render_surface_layer->replica_layer()->mask_layer() : NULL;
  if (replica_mask_layer) {
    *did_paint_content |= replica_mask_layer->Update(queue, NULL);
    *need_more_updates |= replica_mask_layer->NeedMoreUpdates();
  }
}
