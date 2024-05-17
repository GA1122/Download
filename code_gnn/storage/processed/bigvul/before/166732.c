bool LargeObjectPage::Contains(Address object) {
  return RoundToBlinkPageStart(GetAddress()) <= object &&
         object < RoundToBlinkPageEnd(GetAddress() + size());
}
