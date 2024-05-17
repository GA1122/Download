IntSize WebViewTest::PrintICBSizeFromPageSize(const FloatSize& page_size) {
  const float ratio = page_size.Height() / (float)page_size.Width();
  const int icb_width =
      floor(page_size.Width() * PrintContext::kPrintingMinimumShrinkFactor);
  const int icb_height = floor(icb_width * ratio);
  return IntSize(icb_width, icb_height);
}
