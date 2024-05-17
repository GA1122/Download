PrintMsg_Print_Params GetCssPrintParams(
    blink::WebFrame* frame,
    int page_index,
    const PrintMsg_Print_Params& page_params) {
  PrintMsg_Print_Params page_css_params = page_params;
  int dpi = GetDPI(&page_params);

  blink::WebSize page_size_in_pixels(
      ConvertUnit(page_params.page_size.width(), dpi, kPixelsPerInch),
      ConvertUnit(page_params.page_size.height(), dpi, kPixelsPerInch));
  int margin_top_in_pixels =
      ConvertUnit(page_params.margin_top, dpi, kPixelsPerInch);
  int margin_right_in_pixels = ConvertUnit(
      page_params.page_size.width() -
      page_params.content_size.width() - page_params.margin_left,
      dpi, kPixelsPerInch);
  int margin_bottom_in_pixels = ConvertUnit(
      page_params.page_size.height() -
      page_params.content_size.height() - page_params.margin_top,
      dpi, kPixelsPerInch);
  int margin_left_in_pixels = ConvertUnit(
      page_params.margin_left,
      dpi, kPixelsPerInch);

  blink::WebSize original_page_size_in_pixels = page_size_in_pixels;

  if (frame) {
    frame->pageSizeAndMarginsInPixels(page_index,
                                      page_size_in_pixels,
                                      margin_top_in_pixels,
                                      margin_right_in_pixels,
                                      margin_bottom_in_pixels,
                                      margin_left_in_pixels);
  }

  int new_content_width = page_size_in_pixels.width -
                          margin_left_in_pixels - margin_right_in_pixels;
  int new_content_height = page_size_in_pixels.height -
                           margin_top_in_pixels - margin_bottom_in_pixels;

  if (new_content_width < 1 || new_content_height < 1) {
    CHECK(frame != NULL);
    page_css_params = GetCssPrintParams(NULL, page_index, page_params);
    return page_css_params;
  }

  page_css_params.content_size =
      gfx::Size(ConvertUnit(new_content_width, kPixelsPerInch, dpi),
                ConvertUnit(new_content_height, kPixelsPerInch, dpi));

  if (original_page_size_in_pixels != page_size_in_pixels) {
    page_css_params.page_size =
        gfx::Size(ConvertUnit(page_size_in_pixels.width, kPixelsPerInch, dpi),
                  ConvertUnit(page_size_in_pixels.height, kPixelsPerInch, dpi));
  } else {
    page_css_params.page_size = page_params.page_size;
  }

  page_css_params.margin_top =
      ConvertUnit(margin_top_in_pixels, kPixelsPerInch, dpi);
  page_css_params.margin_left =
      ConvertUnit(margin_left_in_pixels, kPixelsPerInch, dpi);
  return page_css_params;
}
