void LocalFrame::StartPrinting(const FloatSize& page_size,
                               const FloatSize& original_page_size,
                               float maximum_shrink_ratio) {
  SetPrinting( true,  true, page_size,
              original_page_size, maximum_shrink_ratio);
}
