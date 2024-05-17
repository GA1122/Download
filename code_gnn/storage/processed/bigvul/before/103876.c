void RenderView::didChangeSelection(bool is_empty_selection) {
#if defined(OS_POSIX)
  if (!handling_input_event_)
      return;

  if (is_empty_selection) {
    last_selection_.clear();
  } else {
    WebFrame* frame = webview()->focusedFrame();
    const std::string& this_selection = frame->selectionAsText().utf8();
    if (this_selection == last_selection_)
      return;
    last_selection_ = this_selection;
  }

  ui::Range range(ui::Range::InvalidRange());
  size_t location, length;
  if (webview()->caretOrSelectionRange(&location, &length)) {
    range.set_start(location);
    range.set_end(location + length);
  }
  Send(new ViewHostMsg_SelectionChanged(routing_id_, last_selection_, range));
#endif   
}
