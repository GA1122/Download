scoped_refptr<ComputedStyle> Document::StyleForPage(int page_index) {
  UpdateDistributionForUnknownReasons();
  return EnsureStyleResolver().StyleForPage(page_index);
}
