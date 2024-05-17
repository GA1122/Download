TestPaintArtifact& TestPaintArtifact::Chunk(DisplayItemClient& client,
                                            const PropertyTreeState& state) {
  auto& chunks = paint_chunks_data_.chunks;
  if (!chunks.IsEmpty())
    chunks.back().end_index = display_item_list_.size();
  chunks.push_back(
      PaintChunk(display_item_list_.size(), 0,
                 PaintChunk::Id(client, DisplayItem::kDrawingFirst), state));
  chunks.back().client_is_just_created = false;
  return *this;
}
