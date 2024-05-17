bool FrameSelection::SetSelectionDeprecated(
    const SelectionInDOMTree& new_selection,
    const SetSelectionOptions& passed_options) {
  SetSelectionOptions::Builder options_builder(passed_options);
  if (ShouldAlwaysUseDirectionalSelection(frame_)) {
    options_builder.SetIsDirectional(true);
  }
  SetSelectionOptions options = options_builder.Build();

  if (granularity_strategy_ && !options.DoNotClearStrategy())
    granularity_strategy_->Clear();
  granularity_ = options.Granularity();

  if (options.ShouldCloseTyping())
    TypingCommand::CloseTyping(frame_);

  if (options.ShouldClearTypingStyle())
    frame_->GetEditor().ClearTypingStyle();

  const SelectionInDOMTree old_selection_in_dom_tree =
      selection_editor_->GetSelectionInDOMTree();
  const bool is_changed = old_selection_in_dom_tree != new_selection;
  const bool should_show_handle = options.ShouldShowHandle();
  if (!is_changed && is_handle_visible_ == should_show_handle &&
      is_directional_ == options.IsDirectional())
    return false;
  if (is_changed)
    selection_editor_->SetSelectionAndEndTyping(new_selection);
  is_directional_ = options.IsDirectional();
  should_shrink_next_tap_ = options.ShouldShrinkNextTap();
  is_handle_visible_ = should_show_handle;
  ScheduleVisualUpdateForPaintInvalidationIfNeeded();

  const Document& current_document = GetDocument();
  frame_->GetEditor().RespondToChangedSelection();
  DCHECK_EQ(current_document, GetDocument());
  return true;
}
