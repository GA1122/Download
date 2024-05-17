bool Layer::Update(ResourceUpdateQueue* queue,
                   const OcclusionTracker<Layer>* occlusion) {
  DCHECK(layer_tree_host_);
  DCHECK_EQ(layer_tree_host_->source_frame_number(),
            paint_properties_.source_frame_number) <<
      "SavePaintProperties must be called for any layer that is painted.";
  return false;
}
