void ScrollableShelfView::ScrollToNewPage(bool forward) {
  float offset = CalculatePageScrollingOffset(forward);
  if (GetShelf()->IsHorizontalAlignment())
    ScrollByXOffset(offset,  true);
  else
    ScrollByYOffset(offset,  true);
}
