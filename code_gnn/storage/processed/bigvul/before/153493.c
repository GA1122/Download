views::View* TabStrip::TargetForRect(views::View* root, const gfx::Rect& rect) {
  CHECK_EQ(root, this);

  if (!views::UsePointBasedTargeting(rect))
    return views::ViewTargeterDelegate::TargetForRect(root, rect);
  const gfx::Point point(rect.CenterPoint());

  if (!touch_layout_) {
    views::View* v = views::ViewTargeterDelegate::TargetForRect(root, rect);
    if (v && v != this && strcmp(v->GetClassName(), Tab::kViewClassName))
      return v;

    views::View* tab = FindTabHitByPoint(point);
    if (tab)
      return tab;
  } else {
    if (new_tab_button_->GetVisible()) {
      views::View* view =
          ConvertPointToViewAndGetEventHandler(this, new_tab_button_, point);
      if (view)
        return view;
    }
    Tab* tab = FindTabForEvent(point);
    if (tab)
      return ConvertPointToViewAndGetEventHandler(this, tab, point);
  }
  return this;
}
