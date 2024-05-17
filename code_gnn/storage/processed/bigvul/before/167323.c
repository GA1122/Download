ScrollableShelfView::~ScrollableShelfView() {
  Shell::Get()->RemoveShellObserver(this);
  GetShelf()->tooltip()->set_shelf_tooltip_delegate(nullptr);
}
