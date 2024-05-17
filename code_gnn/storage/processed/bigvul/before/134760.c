void GuestViewBase::WebContentsDestroyed() {
  GuestDestroyed();

  delete this;
}
