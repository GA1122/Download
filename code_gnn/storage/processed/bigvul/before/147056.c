void WebLocalFrameImpl::RequestFind(int identifier,
                                    const WebString& search_text,
                                    const WebFindOptions& options) {
  if (!HasVisibleContent() && !options.force) {
    Client()->ReportFindInPageMatchCount(identifier, 0  ,
                                         true  );
    return;
  }

  WebRange current_selection = SelectionRange();
  bool result = false;
  bool active_now = false;

  if (IsFocused() || options.find_next) {
    result = Find(identifier, search_text, options, false  ,
                  &active_now);
  }

  if (result && !options.find_next) {
    Client()->ReportFindInPageMatchCount(identifier, 1  ,
                                         false  );
  }

  if (  options.find_next &&   current_selection.IsNull() &&
        !(result && !active_now)) {
    IncreaseMatchCount(0, identifier);
    return;
  }

  EnsureTextFinder().StartScopingStringMatches(identifier, search_text,
                                               options);
}
