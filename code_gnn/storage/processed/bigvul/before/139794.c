OpenPDFInReaderView::~OpenPDFInReaderView() {
  if (bubble_)
    bubble_->GetWidget()->RemoveObserver(this);
}
