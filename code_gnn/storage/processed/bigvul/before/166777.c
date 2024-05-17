void LargeObjectPage::Sweep() {
  GetHeapObjectHeader()->Unmark();
  Arena()->GetThreadState()->Heap().HeapStats().IncreaseMarkedObjectSize(
      size());
}
