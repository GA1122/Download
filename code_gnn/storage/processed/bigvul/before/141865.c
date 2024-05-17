void AutofillPopupBaseView::DoUpdateBoundsAndRedrawPopup() {
  gfx::Size size = GetPreferredSize();
  gfx::Rect element_bounds = gfx::ToEnclosingRect(delegate()->element_bounds());
  element_bounds.Inset( 0,  -kElementBorderPadding);

  gfx::Rect popup_bounds = PopupViewCommon().CalculatePopupBounds(
      size.width(), size.height(), element_bounds, delegate()->container_view(),
      delegate()->IsRTL());
  popup_bounds.Inset(-GetWidget()->GetRootView()->border()->GetInsets());
  GetWidget()->SetBounds(popup_bounds);

  Layout();
  SetClipPath();
   SchedulePaint();
 }
