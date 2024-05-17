void Layer::SetDrawCheckerboardForMissingTiles(bool checkerboard) {
  DCHECK(IsPropertyChangeAllowed());
  if (draw_checkerboard_for_missing_tiles_ == checkerboard)
    return;
  draw_checkerboard_for_missing_tiles_ = checkerboard;
  SetNeedsCommit();
}
