void Tab::OnBoundsChanged(const gfx::Rect& previous_bounds) {
  const SkPath path = tab_style_->GetPath(TabStyle::PathType::kHighlight, 1.0);
  SetProperty(views::kHighlightPathKey, new SkPath(path));
}
