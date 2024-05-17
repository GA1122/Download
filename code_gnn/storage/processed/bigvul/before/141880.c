 void AutofillPopupBaseView::OnNativeFocusChanged(gfx::NativeView focused_now) {
   if (GetWidget() && GetWidget()->GetNativeView() != focused_now)
     HideController();
}
