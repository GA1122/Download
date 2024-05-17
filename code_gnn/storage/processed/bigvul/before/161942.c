bool PDFShouldDisableScalingBasedOnPreset(
    const blink::WebPrintPresetOptions& options,
    const PrintMsg_Print_Params& params,
    bool ignore_page_size) {
  if (options.is_scaling_disabled)
    return true;

  if (!options.is_page_size_uniform)
    return false;

  int dpi = GetDPI(&params);
  if (!dpi) {
    return true;
  }

  if (ignore_page_size)
    return false;

  blink::WebSize page_size(
      ConvertUnit(params.page_size.width(), dpi, kPointsPerInch),
      ConvertUnit(params.page_size.height(), dpi, kPointsPerInch));
  return options.uniform_page_size == page_size;
}
