bool HTMLInputElement::HasPendingActivity() const {
  return ImageLoader() && ImageLoader()->HasPendingActivity();
}
