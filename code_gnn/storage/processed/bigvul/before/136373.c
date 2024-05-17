  PaintArtifact Build() {
    return PaintArtifact(DisplayItemList(0), std::move(data_));
  }
