void DrawingBuffer::SetFilterQuality(SkFilterQuality filter_quality) {
  if (filter_quality_ != filter_quality) {
    filter_quality_ = filter_quality;
    if (layer_)
      layer_->SetNearestNeighbor(filter_quality == kNone_SkFilterQuality);
  }
}
