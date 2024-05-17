void HTMLMediaElement::videoWillBeDrawnToCanvas() const {
  DCHECK(isHTMLVideoElement());
  UseCounter::count(document(), UseCounter::VideoInCanvas);
  if (m_autoplayUmaHelper->hasSource() && !m_autoplayUmaHelper->isVisible())
    UseCounter::count(document(), UseCounter::HiddenAutoplayedVideoInCanvas);
}
