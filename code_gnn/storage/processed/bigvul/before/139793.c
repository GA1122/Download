 void OpenPDFInReaderView::ShowBubble() {
  if (bubble_)
    return;

  DCHECK(model_);
  bubble_ = new OpenPDFInReaderBubbleView(this, model_);
  views::BubbleDelegateView::CreateBubble(bubble_);
  bubble_->GetWidget()->AddObserver(this);
  bubble_->GetWidget()->Show();
}
