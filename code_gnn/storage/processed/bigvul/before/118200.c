gfx::Rect AutofillDialogViews::OverlayView::ContentBoundsSansBubbleBorder() {
  gfx::Rect bounds = GetContentsBounds();
  int bubble_width = 5;
  if (GetBubbleBorder())
    bubble_width = GetBubbleBorder()->GetBorderThickness();
  bounds.Inset(bubble_width, bubble_width, bubble_width, bubble_width);
  return bounds;
}
