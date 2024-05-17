void BrowserView::Layout() {
  if (ignore_layout_)
    return;
  views::View::Layout();

  LayoutStatusBubble();

  MaybeStackBookmarkBarAtTop();
}
