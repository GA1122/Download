bool WebPluginDelegatePepper::CalculatePrintedPageDimensions(
    int page_number,
    NPPPrintExtensions* print_extensions,
    gfx::Size* page_dimensions) {
  int32 width_in_pixels = 0;
  int32 height_in_pixels = 0;
  NPError err = print_extensions->getRasterDimensions(
      instance()->npp(), page_number, &width_in_pixels, &height_in_pixels);
  if (err != NPERR_NO_ERROR)
    return false;

  DCHECK(width_in_pixels && height_in_pixels);
  page_dimensions->SetSize(width_in_pixels, height_in_pixels);
  return true;
}
