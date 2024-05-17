void AXTree::AddObserver(AXTreeObserver* observer) {
  observers_.AddObserver(observer);
}
