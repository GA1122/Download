bool LargeObjectPage::isEmpty() {
  return !heapObjectHeader()->isMarked();
}
