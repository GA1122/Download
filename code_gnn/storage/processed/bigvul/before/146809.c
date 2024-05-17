Element* Document::PointerLockElement() const {
  if (!GetPage() || GetPage()->GetPointerLockController().LockPending())
    return 0;
  if (Element* element = GetPage()->GetPointerLockController().GetElement()) {
    if (element->GetDocument() == this)
      return element;
  }
  return 0;
}
