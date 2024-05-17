void PaintController::GenerateRasterInvalidations(PaintChunk& new_chunk) {
  if (RuntimeEnabledFeatures::DisableRasterInvalidationEnabled())
    return;

  DCHECK(RuntimeEnabledFeatures::SlimmingPaintV175Enabled());
  if (new_chunk.begin_index >=
      current_cached_subsequence_begin_index_in_new_list_)
    return;

  if (!new_chunk.is_cacheable)
    return;

  const auto& old_chunks = current_paint_artifact_.PaintChunks();
  while (next_chunk_to_match_ < old_chunks.size()) {
    const PaintChunk& old_chunk = old_chunks[next_chunk_to_match_];
    if (new_chunk.Matches(old_chunk)) {
      GenerateRasterInvalidationsComparingChunks(new_chunk, old_chunk);
      ++next_chunk_to_match_;
      return;
    }

    if (old_chunk.is_cacheable) {
      auto it = out_of_order_chunk_indices_.find(&old_chunk.id.client);
      Vector<size_t>& indices =
          it == out_of_order_chunk_indices_.end()
              ? out_of_order_chunk_indices_
                    .insert(&old_chunk.id.client, Vector<size_t>())
                    .stored_value->value
              : it->value;
      indices.push_back(next_chunk_to_match_);
    }
    ++next_chunk_to_match_;
  }

  auto it = out_of_order_chunk_indices_.find(&new_chunk.id.client);
  if (it != out_of_order_chunk_indices_.end()) {
    for (size_t i : it->value) {
      if (new_chunk.Matches(old_chunks[i])) {
        GenerateRasterInvalidationsComparingChunks(new_chunk, old_chunks[i]);
        return;
      }
    }
  }
}
