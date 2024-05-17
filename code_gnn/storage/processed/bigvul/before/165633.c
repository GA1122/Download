void Location::setSearch(LocalDOMWindow* current_window,
                         LocalDOMWindow* entered_window,
                         const String& search,
                         ExceptionState& exception_state) {
  KURL url = GetDocument()->Url();
  url.SetQuery(search);
  SetLocation(url.GetString(), current_window, entered_window,
              &exception_state);
}
