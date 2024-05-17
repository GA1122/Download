void ScrollableShelfView::ChildPreferredSizeChanged(views::View* child) {
  if (GetShelf()->IsHorizontalAlignment())
    ScrollByXOffset(0,  false);
  else
    ScrollByYOffset(0,  false);
}
