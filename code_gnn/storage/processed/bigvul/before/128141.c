void TrayCast::UpdateAfterShelfAlignmentChange(ShelfAlignment alignment) {
  if (tray_)
    tray_->UpdateAlignment(alignment);
}
