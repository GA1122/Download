void LargeObjectPage::sweep() {
  heapObjectHeader()->unmark();
  arena()->getThreadState()->increaseMarkedObjectSize(size());
}
