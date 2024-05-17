void CompositedLayerRasterInvalidator::Generate(
    const PaintArtifact& paint_artifact,
    const gfx::Rect& layer_bounds,
    const PropertyTreeState& layer_state,
    const FloatSize& visual_rect_subpixel_offset) {
  Generate(paint_artifact, paint_artifact.PaintChunks(), layer_bounds,
           layer_state, visual_rect_subpixel_offset);
}
