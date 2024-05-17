void PrintWebViewHelper::UpdateFrameAndViewFromCssPageLayout(
    WebKit::WebFrame* frame,
    const WebKit::WebNode& node,
    PrepareFrameAndViewForPrint* prepare,
    const PrintMsg_Print_Params& params,
    bool ignore_css_margins) {
  if (PrintingNodeOrPdfFrame(frame, node))
    return;
  bool fit_to_page = ignore_css_margins &&
                     params.print_scaling_option ==
                          WebKit::WebPrintScalingOptionFitToPrintableArea;
  PrintMsg_Print_Params print_params = CalculatePrintParamsForCss(
      frame, 0, params, ignore_css_margins, fit_to_page, NULL);
  prepare->UpdatePrintParams(print_params);
}
