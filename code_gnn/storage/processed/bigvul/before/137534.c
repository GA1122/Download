blink::WebPrintScalingOption GetPrintScalingOption(
    blink::WebLocalFrame* frame,
    const blink::WebNode& node,
    bool source_is_html,
    const base::DictionaryValue& job_settings,
    const PrintMsg_Print_Params& params) {
  if (params.print_to_pdf)
    return blink::WebPrintScalingOptionSourceSize;

  if (!source_is_html) {
    if (!FitToPageEnabled(job_settings))
      return blink::WebPrintScalingOptionNone;

    bool no_plugin_scaling = PDFShouldDisableScaling(frame, node, params);
    if (params.is_first_request && no_plugin_scaling)
      return blink::WebPrintScalingOptionNone;
  }
  return blink::WebPrintScalingOptionFitToPrintableArea;
}
