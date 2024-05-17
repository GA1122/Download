void OpenPDFInReaderView::OnWidgetDestroying(views::Widget* widget) {
  if (!bubble_)
    return;

  bubble_->GetWidget()->RemoveObserver(this);
  bubble_ = NULL;
}
