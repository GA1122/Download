const PaintArtifact& TestPaintArtifact::Build() {
  if (built_)
    return paint_artifact_;

  if (!paint_chunks_data_.chunks.IsEmpty())
    paint_chunks_data_.chunks.back().end_index = display_item_list_.size();
  paint_artifact_ = PaintArtifact(std::move(display_item_list_),
                                  std::move(paint_chunks_data_));
  built_ = true;
  return paint_artifact_;
}
