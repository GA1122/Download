void PrintWebViewHelper::ComputePageLayoutInPointsForCss(
    blink::WebFrame* frame,
    int page_index,
    const PrintMsg_Print_Params& page_params,
    bool ignore_css_margins,
    double* scale_factor,
    PageSizeMargins* page_layout_in_points) {
  PrintMsg_Print_Params params = CalculatePrintParamsForCss(
      frame, page_index, page_params, ignore_css_margins,
      page_params.print_scaling_option ==
          blink::WebPrintScalingOptionFitToPrintableArea,
      scale_factor);
  CalculatePageLayoutFromPrintParams(params, page_layout_in_points);
}
