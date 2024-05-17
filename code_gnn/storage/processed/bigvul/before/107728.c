bool FrameLoader::AllAncestorsAreComplete() const {
  for (Frame* ancestor = frame_; ancestor;
       ancestor = ancestor->Tree().Parent()) {
    if (ancestor->IsLoading())
      return false;
  }
  return true;
}
