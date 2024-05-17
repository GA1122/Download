PaintArtifactCompositor::CompositedLayerForPendingLayer(
    const PaintArtifact& paint_artifact,
    const PendingLayer& pending_layer,
    gfx::Vector2dF& layer_offset,
    Vector<std::unique_ptr<ContentLayerClientImpl>>& new_content_layer_clients,
    Vector<scoped_refptr<cc::Layer>>& new_scroll_hit_test_layers) {
  auto paint_chunks =
      paint_artifact.GetPaintChunkSubset(pending_layer.paint_chunk_indices);
  DCHECK(paint_chunks.size());
  const PaintChunk& first_paint_chunk = paint_chunks[0];
  DCHECK(first_paint_chunk.size());

  if (scoped_refptr<cc::Layer> foreign_layer = ForeignLayerForPaintChunk(
          paint_artifact, first_paint_chunk, layer_offset)) {
    DCHECK_EQ(paint_chunks.size(), 1u);
    if (extra_data_for_testing_enabled_)
      extra_data_for_testing_->content_layers.push_back(foreign_layer);
    return foreign_layer;
  }

  if (scoped_refptr<cc::Layer> scroll_layer = ScrollHitTestLayerForPendingLayer(
          paint_artifact, pending_layer, layer_offset)) {
    new_scroll_hit_test_layers.push_back(scroll_layer);
    if (extra_data_for_testing_enabled_)
      extra_data_for_testing_->scroll_hit_test_layers.push_back(scroll_layer);
    return scroll_layer;
  }

  std::unique_ptr<ContentLayerClientImpl> content_layer_client =
      ClientForPaintChunk(first_paint_chunk);

  gfx::Rect cc_combined_bounds(EnclosingIntRect(pending_layer.bounds));
  layer_offset = cc_combined_bounds.OffsetFromOrigin();

  auto cc_layer = content_layer_client->UpdateCcPictureLayer(
      paint_artifact, paint_chunks, cc_combined_bounds,
      pending_layer.property_tree_state);
  new_content_layer_clients.push_back(std::move(content_layer_client));
  if (extra_data_for_testing_enabled_)
    extra_data_for_testing_->content_layers.push_back(cc_layer);

  cc_layer->SetContentsOpaque(pending_layer.rect_known_to_be_opaque.Contains(
      FloatRect(EnclosingIntRect(pending_layer.bounds))));

  return cc_layer;
}
