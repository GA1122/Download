void LargeObjectPage::removeFromHeap() {
  static_cast<LargeObjectArena*>(arena())->freeLargeObjectPage(this);
}
