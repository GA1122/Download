static scoped_refptr<cc::Layer> ForeignLayerForPaintChunk(
    const PaintArtifact& paint_artifact,
    const PaintChunk& paint_chunk,
    gfx::Vector2dF& layer_offset) {
  if (paint_chunk.size() != 1)
    return nullptr;

  const auto& display_item =
      paint_artifact.GetDisplayItemList()[paint_chunk.begin_index];
  if (!display_item.IsForeignLayer())
    return nullptr;

  const auto& foreign_layer_display_item =
      static_cast<const ForeignLayerDisplayItem&>(display_item);
  layer_offset = gfx::Vector2dF(foreign_layer_display_item.Location().X(),
                                foreign_layer_display_item.Location().Y());
  scoped_refptr<cc::Layer> layer = foreign_layer_display_item.GetLayer();
  DCHECK(layer->bounds() ==
         static_cast<gfx::Size>(foreign_layer_display_item.Bounds()))
      << "\n  layer bounds: " << layer->bounds().ToString()
      << "\n  display item bounds: " << foreign_layer_display_item.Bounds();
  DCHECK(layer->DrawsContent());
  return layer;
}
