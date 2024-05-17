void ShelfLayoutManager::AdjustBoundsBasedOnAlignment(int inset,
                                                      gfx::Rect* bounds) const {
  switch (alignment_) {
    case SHELF_ALIGNMENT_BOTTOM:
      bounds->Inset(gfx::Insets(0, 0, inset, 0));
      break;
    case SHELF_ALIGNMENT_LEFT:
      bounds->Inset(gfx::Insets(0, inset, 0, 0));
      break;
    case SHELF_ALIGNMENT_RIGHT:
      bounds->Inset(gfx::Insets(0, 0, 0, inset));
      break;
  }
}
