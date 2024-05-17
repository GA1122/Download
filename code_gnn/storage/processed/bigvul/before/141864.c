void AutofillPopupBaseView::DoHide() {
  delegate_ = NULL;

  RemoveWidgetObservers();

  if (GetWidget()) {
    GetWidget()->Close();
  } else {
    delete this;
  }
}
