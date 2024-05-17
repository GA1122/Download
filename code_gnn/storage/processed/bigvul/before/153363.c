void TabStrip::AddedToWidget() {
  GetWidget()->AddObserver(this);
}
