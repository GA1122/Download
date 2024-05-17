void InfoBarContainer::Layout() {
  int top = 0;
  for (int i = 0; i < GetChildViewCount(); ++i) {
    views::View* child = GetChildViewAt(i);
    gfx::Size ps = child->GetPreferredSize();
    child->SetBounds(0, top, width(), ps.height());
    top += ps.height();
  }
}
