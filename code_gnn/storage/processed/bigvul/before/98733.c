void WebPluginDelegatePepper::PrintEnd() {
  NPPPrintExtensions* print_extensions = GetPrintExtensions();
  if (print_extensions)
    print_extensions->printEnd(instance()->npp());
  current_printable_area_ = gfx::Rect();
}
