void PaintController::CheckDuplicatePaintChunkId(const PaintChunk::Id& id) {
  if (IsSkippingCache())
    return;

  auto it = new_paint_chunk_indices_by_client_.find(&id.client);
  if (it != new_paint_chunk_indices_by_client_.end()) {
    const auto& indices = it->value;
    for (auto index : indices) {
      const auto& chunk = new_paint_chunks_.PaintChunkAt(index);
      if (chunk.id == id) {
        ShowDebugData();
        NOTREACHED() << "New paint chunk id " << id.ToString().Utf8().data()
                     << " has duplicated id with previous chuck "
                     << chunk.ToString().Utf8().data();
      }
    }
  }
}
