void CalculatePageLayoutFromPrintParams(
    const PrintMsg_Print_Params& params,
    double scale_factor,
    PageSizeMargins* page_layout_in_points) {
  bool fit_to_page = IsWebPrintScalingOptionFitToPage(params);
  int dpi = GetDPI(&params);
  int content_width = params.content_size.width();
  int content_height = params.content_size.height();
  bool scale = fit_to_page || params.print_to_pdf;
  if (scale && scale_factor >= PrintRenderFrameHelper::kEpsilon) {
    content_width =
        static_cast<int>(static_cast<double>(content_width) * scale_factor);
    content_height =
        static_cast<int>(static_cast<double>(content_height) * scale_factor);
  }

  int margin_bottom =
      params.page_size.height() - content_height - params.margin_top;
  int margin_right =
      params.page_size.width() - content_width - params.margin_left;

  page_layout_in_points->content_width =
      ConvertUnit(content_width, dpi, kPointsPerInch);
  page_layout_in_points->content_height =
      ConvertUnit(content_height, dpi, kPointsPerInch);
  page_layout_in_points->margin_top =
      ConvertUnit(params.margin_top, dpi, kPointsPerInch);
  page_layout_in_points->margin_right =
      ConvertUnit(margin_right, dpi, kPointsPerInch);
  page_layout_in_points->margin_bottom =
      ConvertUnit(margin_bottom, dpi, kPointsPerInch);
  page_layout_in_points->margin_left =
      ConvertUnit(params.margin_left, dpi, kPointsPerInch);
}
