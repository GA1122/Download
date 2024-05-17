scoped_refptr<ComputedStyle> Document::StyleForPage(int page_index) {
  UpdateDistribution();
  return EnsureStyleResolver().StyleForPage(page_index);
}
