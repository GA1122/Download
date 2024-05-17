bool NormalPage::Contains(Address addr) {
  Address blink_page_start = RoundToBlinkPageStart(GetAddress());
  DCHECK_EQ(blink_page_start, GetAddress() - kBlinkGuardPageSize);
  return blink_page_start <= addr && addr < blink_page_start + kBlinkPageSize;
}
