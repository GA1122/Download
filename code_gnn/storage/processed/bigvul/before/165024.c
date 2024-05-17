void HTMLCanvasElement::PageVisibilityChanged() {
  bool hidden = !GetPage()->IsPageVisible();
  SetSuspendOffscreenCanvasAnimation(hidden);

  if (!context_)
    return;

  context_->SetIsHidden(hidden);
  if (hidden && Is3d())
    DiscardResourceProvider();
}
