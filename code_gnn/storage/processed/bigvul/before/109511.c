WebKit::WebPrintScalingOption PrintWebViewHelper::GetPrintScalingOption(
    bool source_is_html, const DictionaryValue& job_settings,
    const PrintMsg_Print_Params& params) {
  DCHECK(!print_for_preview_);

  if (params.print_to_pdf)
    return WebKit::WebPrintScalingOptionSourceSize;

  if (!source_is_html) {
    if (!FitToPageEnabled(job_settings))
      return WebKit::WebPrintScalingOptionNone;

    bool no_plugin_scaling =
        print_preview_context_.frame()->isPrintScalingDisabledForPlugin(
            print_preview_context_.node());

    if (params.is_first_request && no_plugin_scaling)
      return WebKit::WebPrintScalingOptionNone;
  }
  return WebKit::WebPrintScalingOptionFitToPrintableArea;
}
