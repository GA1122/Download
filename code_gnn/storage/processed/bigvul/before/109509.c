gfx::Size PrepareFrameAndViewForPrint::GetPrintCanvasSize() const {
  return gfx::Size(web_print_params_.printContentArea.width,
                   web_print_params_.printContentArea.height);
}
