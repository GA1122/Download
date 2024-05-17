void AXTree::RemoveObserver(const AXTreeObserver* observer) {
  observers_.RemoveObserver(observer);
}
