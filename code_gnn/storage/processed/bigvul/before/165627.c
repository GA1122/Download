void Location::setHash(LocalDOMWindow* current_window,
                       LocalDOMWindow* entered_window,
                       const String& hash,
                       ExceptionState& exception_state) {
  KURL url = GetDocument()->Url();
  String old_fragment_identifier = url.FragmentIdentifier();
  String new_fragment_identifier = hash;
  if (hash[0] == '#')
    new_fragment_identifier = hash.Substring(1);
  url.SetFragmentIdentifier(new_fragment_identifier);
  if (EqualIgnoringNullity(old_fragment_identifier, url.FragmentIdentifier()))
    return;
  SetLocation(url.GetString(), current_window, entered_window,
              &exception_state);
}
