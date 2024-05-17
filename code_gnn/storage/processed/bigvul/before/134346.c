views::View* TabStrip::GetEventHandlerForRect(const gfx::Rect& rect) {
  if (!views::UsePointBasedTargeting(rect))
    return View::GetEventHandlerForRect(rect);
  const gfx::Point point(rect.CenterPoint());

  if (!touch_layout_.get()) {
    views::View* v = View::GetEventHandlerForRect(rect);
    if (v && v != this && strcmp(v->GetClassName(), Tab::kViewClassName))
      return v;

    views::View* tab = FindTabHitByPoint(point);
    if (tab)
      return tab;
  } else {
    if (newtab_button_->visible()) {
      views::View* view =
          ConvertPointToViewAndGetEventHandler(this, newtab_button_, point);
      if (view)
        return view;
    }
    Tab* tab = FindTabForEvent(point);
    if (tab)
      return ConvertPointToViewAndGetEventHandler(this, tab, point);
  }
  return this;
}
