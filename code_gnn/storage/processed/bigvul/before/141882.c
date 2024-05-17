void AutofillPopupBaseView::RemoveWidgetObservers() {
  if (parent_widget_)
    parent_widget_->RemoveObserver(this);
  GetWidget()->RemoveObserver(this);

   views::WidgetFocusManager::GetInstance()->RemoveFocusChangeListener(this);
 }
