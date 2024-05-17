void PaintChunksToCcLayer::ConvertInto(
    const PaintChunkSubset& paint_chunks,
    const PropertyTreeState& layer_state,
    const gfx::Vector2dF& layer_offset,
    const FloatSize& visual_rect_subpixel_offset,
    const DisplayItemList& display_items,
    cc::DisplayItemList& cc_list) {
  if (RuntimeEnabledFeatures::DisablePaintChunksToCcLayerEnabled())
    return;
  ConversionContext(layer_state, layer_offset, visual_rect_subpixel_offset,
                    cc_list)
      .Convert(paint_chunks, display_items);
}
