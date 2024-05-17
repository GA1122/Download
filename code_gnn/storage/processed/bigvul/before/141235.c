void Document::SetPrinting(PrintingState state) {
  bool was_printing = Printing();
  printing_ = state;
  bool is_printing = Printing();

  if ((was_printing != is_printing) && documentElement() && GetFrame() &&
      !GetFrame()->IsMainFrame() && GetFrame()->Owner() &&
      GetFrame()->Owner()->IsDisplayNone()) {
    documentElement()->LazyReattachIfAttached();
  }
}
