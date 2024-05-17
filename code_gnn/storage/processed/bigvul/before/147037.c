void WebLocalFrameImpl::PageSizeAndMarginsInPixels(int page_index,
                                                   WebDoubleSize& page_size,
                                                   int& margin_top,
                                                   int& margin_right,
                                                   int& margin_bottom,
                                                   int& margin_left) {
  DoubleSize size = page_size;
  GetFrame()->GetDocument()->PageSizeAndMarginsInPixels(
      page_index, size, margin_top, margin_right, margin_bottom, margin_left);
  page_size = size;
}
