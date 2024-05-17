blink::WebPrintScalingOption GetPrintScalingOption(
    blink::WebLocalFrame* frame,
    const blink::WebNode& node,
    bool source_is_html,
    const base::DictionaryValue& job_settings,
    const PrintMsg_Print_Params& params) {
  if (params.print_to_pdf)
    return blink::kWebPrintScalingOptionSourceSize;

  if (!source_is_html) {
    if (!FitToPageEnabled(job_settings))
      return blink::kWebPrintScalingOptionNone;

    bool no_plugin_scaling = PDFShouldDisableScaling(frame, node, params, true);
    if (params.is_first_request && no_plugin_scaling)
      return blink::kWebPrintScalingOptionNone;
  }
  return blink::kWebPrintScalingOptionFitToPrintableArea;
}
