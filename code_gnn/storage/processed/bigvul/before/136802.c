bool LocalDOMWindow::find(const String& string,
                          bool case_sensitive,
                          bool backwards,
                          bool wrap,
                          bool whole_word,
                          bool  ,
                          bool  ) const {
  if (!IsCurrentlyDisplayedInFrame())
    return false;

  document()->UpdateStyleAndLayoutIgnorePendingStylesheets();

  FindOptions options =
      (backwards ? kBackwards : 0) | (case_sensitive ? 0 : kCaseInsensitive) |
      (wrap ? kWrapAround : 0) | (whole_word ? kWholeWord | kAtWordStarts : 0);
  return GetFrame()->GetEditor().FindString(string, options);
}
