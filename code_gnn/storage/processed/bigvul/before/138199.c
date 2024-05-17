void AXObject::selectionChanged() {
  if (AXObject* parent = parentObjectIfExists())
    parent->selectionChanged();
}
