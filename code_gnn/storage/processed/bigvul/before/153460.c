void TabStrip::RemovedFromWidget() {
  GetWidget()->RemoveObserver(this);
}
