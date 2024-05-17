bool PDFShouldDisableScalingBasedOnPreset(
    const blink::WebPrintPresetOptions& options,
    const PrintMsg_Print_Params& params) {
  if (options.isScalingDisabled)
    return true;

  if (!options.isPageSizeUniform)
    return false;

  int dpi = GetDPI(&params);
  if (!dpi) {
    return true;
  }

  blink::WebSize page_size(
      ConvertUnit(params.page_size.width(), dpi, kPointsPerInch),
      ConvertUnit(params.page_size.height(), dpi, kPointsPerInch));
  return options.uniformPageSize == page_size;
}
