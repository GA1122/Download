gfx::Size InfoBarContainer::GetPreferredSize() {
  int height = 0;
  for (int i = 0; i < GetChildViewCount(); ++i)
    height += GetChildViewAt(i)->GetPreferredSize().height();
  return gfx::Size(0, height);
}
