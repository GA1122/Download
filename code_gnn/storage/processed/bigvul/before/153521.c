TabStyle::SeparatorOpacities GM2TabStyle::GetSeparatorOpacities(
    bool for_layout) const {
  float leading_opacity, trailing_opacity;
  if (tab_->IsActive()) {
    leading_opacity = trailing_opacity = 0;
  } else {
    const Tab* subsequent_tab = tab_->controller()->GetAdjacentTab(tab_, 1);
    const Tab* previous_tab = tab_->controller()->GetAdjacentTab(tab_, -1);

    auto adjacent_hover_value = [for_layout](const Tab* tab) {
      if (for_layout || !tab || tab->IsActive())
        return 0.f;
      auto* tab_style = static_cast<const GM2TabStyle*>(tab->tab_style());
      return float{tab_style->GetHoverAnimationValue()};
    };
    const float hover_value = GetHoverAnimationValue();
    trailing_opacity =
        1.f - std::max(hover_value, adjacent_hover_value(subsequent_tab));
    leading_opacity =
        1.f - std::max(hover_value, adjacent_hover_value(previous_tab));

    if (tab_->IsSelected()) {
      if (previous_tab && !previous_tab->IsSelected())
        leading_opacity = 0;
      if (subsequent_tab && !subsequent_tab->IsSelected())
        trailing_opacity = 0;
    } else if (tab_->controller()->HasVisibleBackgroundTabShapes()) {
      if (previous_tab && previous_tab->IsSelected())
        leading_opacity = 0;
      if (subsequent_tab && subsequent_tab->IsSelected())
        trailing_opacity = 0;
    }
  }

  const gfx::Rect target_bounds =
      tab_->controller()->GetTabAnimationTargetBounds(tab_);
  const int tab_width = std::max(tab_->width(), target_bounds.width());
  const float target_opacity =
      float{std::min(std::abs(tab_->x() - target_bounds.x()), tab_width)} /
      tab_width;
  if (tab_->controller()->IsFirstVisibleTab(tab_))
    leading_opacity = target_opacity;
  if (tab_->controller()->IsLastVisibleTab(tab_) &&
      tab_->controller()->HasVisibleBackgroundTabShapes())
    trailing_opacity = target_opacity;

  if (base::i18n::IsRTL())
    std::swap(leading_opacity, trailing_opacity);
  return {leading_opacity, trailing_opacity};
}
