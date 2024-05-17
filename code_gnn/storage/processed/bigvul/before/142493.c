void ShelfLayoutManager::UpdateBoundsAndOpacity(
    const TargetBounds& target_bounds,
    bool animate,
    ui::ImplicitAnimationObserver* observer) {
  StatusAreaWidget* status_widget = shelf_widget_->status_area_widget();
  base::AutoReset<bool> auto_reset_updating_bounds(&updating_bounds_, true);
  {
    ui::ScopedLayerAnimationSettings shelf_animation_setter(
        GetLayer(shelf_widget_)->GetAnimator());
    ui::ScopedLayerAnimationSettings status_animation_setter(
        GetLayer(status_widget)->GetAnimator());
    if (animate) {
      auto duration = base::TimeDelta::FromMilliseconds(kAnimationDurationMS);
      shelf_animation_setter.SetTransitionDuration(duration);
      shelf_animation_setter.SetTweenType(gfx::Tween::EASE_OUT);
      shelf_animation_setter.SetPreemptionStrategy(
          ui::LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);
      status_animation_setter.SetTransitionDuration(duration);
      status_animation_setter.SetTweenType(gfx::Tween::EASE_OUT);
      status_animation_setter.SetPreemptionStrategy(
          ui::LayerAnimator::IMMEDIATELY_ANIMATE_TO_NEW_TARGET);
    } else {
      StopAnimating();
      shelf_animation_setter.SetTransitionDuration(base::TimeDelta());
      status_animation_setter.SetTransitionDuration(base::TimeDelta());
    }
    if (observer)
      status_animation_setter.AddObserver(observer);

    GetLayer(shelf_widget_)->SetOpacity(target_bounds.opacity);
    gfx::Rect shelf_bounds = target_bounds.shelf_bounds;
    ::wm::ConvertRectToScreen(shelf_widget_->GetNativeWindow()->parent(),
                              &shelf_bounds);
    shelf_widget_->SetBounds(shelf_bounds);

    GetLayer(status_widget)->SetOpacity(target_bounds.status_opacity);

    if (!target_bounds.status_opacity)
      status_widget->Hide();

    gfx::Rect status_bounds = target_bounds.status_bounds_in_shelf;
    status_bounds.Offset(target_bounds.shelf_bounds.OffsetFromOrigin());
    ::wm::ConvertRectToScreen(status_widget->GetNativeWindow()->parent(),
                              &status_bounds);
    status_widget->SetBounds(status_bounds);

    display_ = display::Screen::GetScreen()->GetDisplayNearestWindow(
        shelf_widget_->GetNativeWindow());
    bool in_overview = Shell::Get()->overview_controller()->IsSelecting();
    if (!in_overview && !state_.IsScreenLocked() &&
        (shelf_->alignment() != SHELF_ALIGNMENT_BOTTOM_LOCKED ||
         display_.work_area() == display_.bounds())) {
      gfx::Insets insets;
      if (!state_.IsAddingSecondaryUser() || IsKeyboardShown())
        insets = target_bounds.work_area_insets;
      Shell::Get()->SetDisplayWorkAreaInsets(shelf_widget_->GetNativeWindow(),
                                             insets);
    }
  }

  gfx::Rect shelf_bounds = gfx::Rect(target_bounds.shelf_bounds.size());
  shelf_widget_->GetContentsView()->SetBorder(views::CreateEmptyBorder(
      shelf_bounds.InsetsFrom(target_bounds.shelf_bounds_in_shelf)));
  shelf_widget_->GetContentsView()->Layout();

  if (target_bounds.status_opacity)
    status_widget->Show();
}
