void AutofillPopupBaseView::OnWidgetDestroying(views::Widget* widget) {
  DCHECK(widget == parent_widget_ || widget == GetWidget());

  RemoveWidgetObservers();

  parent_widget_ = nullptr;

  HideController();
}
