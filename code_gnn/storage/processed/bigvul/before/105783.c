gfx::Size Label::GetPreferredSize() {
  if (!IsVisible() && collapse_when_hidden_)
    return gfx::Size();

  gfx::Size prefsize(GetTextSize());
  gfx::Insets insets = GetInsets();
  prefsize.Enlarge(insets.width(), insets.height());
  return prefsize;
}
