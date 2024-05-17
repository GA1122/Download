void AutofillDialogViews::DetailsContainerView::OnBoundsChanged(
    const gfx::Rect& previous_bounds) {
  bounds_changed_callback_.Run();
}
