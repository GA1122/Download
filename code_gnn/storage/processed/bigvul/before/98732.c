int WebPluginDelegatePepper::PrintBegin(const gfx::Rect& printable_area,
                                        int printer_dpi) {
  int32 num_pages = 0;
  NPPPrintExtensions* print_extensions = GetPrintExtensions();
  if (print_extensions) {
    NPRect np_printable_area = {0};
    np_printable_area.left = printable_area.x();
    np_printable_area.top = printable_area.y();
    np_printable_area.right = np_printable_area.left + printable_area.width();
    np_printable_area.bottom = np_printable_area.top + printable_area.height();
    if (NPERR_NO_ERROR == print_extensions->printBegin(instance()->npp(),
                                                       &np_printable_area,
                                                       printer_dpi,
                                                       &num_pages)) {
      current_printable_area_ = printable_area;
    }
  }
  return num_pages;
}
