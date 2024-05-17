void LauncherView::LauncherItemAdded(int model_index) {
  model_index = CancelDrag(model_index);
  views::View* view = CreateViewForItem(model_->items()[model_index]);
  AddChildView(view);
  view->layer()->SetOpacity(0);
  view_model_->Add(view, model_index);

  IdealBounds ideal_bounds;
  CalculateIdealBounds(&ideal_bounds);
  view->SetBoundsRect(view_model_->ideal_bounds(model_index));

  AnimateToIdealBounds();
  if (model_index <= last_visible_index_) {
    bounds_animator_->SetAnimationDelegate(
        view, new StartFadeAnimationDelegate(this, view), true);
  } else {
    view->layer()->SetOpacity(1.0f);
  }
}
