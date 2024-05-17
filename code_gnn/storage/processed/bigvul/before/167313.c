ScrollableShelfView::ScrollableShelfView(ShelfModel* model, Shelf* shelf)
    : shelf_view_(new ShelfView(model, shelf)) {
  Shell::Get()->AddShellObserver(this);
  set_allow_deactivate_on_esc(true);
}
