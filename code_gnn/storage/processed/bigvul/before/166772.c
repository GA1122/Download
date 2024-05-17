void NormalPage::RemoveFromHeap() {
  ArenaForNormalPage()->FreePage(this);
}
