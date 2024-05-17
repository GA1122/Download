bool NormalPage::contains(Address addr) {
  Address blinkPageStart = roundToBlinkPageStart(getAddress());
  ASSERT(blinkPageStart == getAddress() - blinkGuardPageSize);
  return blinkPageStart <= addr && addr < blinkPageStart + blinkPageSize;
}
