void WebLocalFrameImpl::PrintPagesForTesting(
    WebCanvas* canvas,
    const WebSize& page_size_in_pixels) {
  DCHECK(print_context_);

  print_context_->SpoolAllPagesWithBoundariesForTesting(
      canvas, FloatSize(page_size_in_pixels.width, page_size_in_pixels.height));
}
