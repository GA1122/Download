void InputMethodController::SetComposition(
    const String& text,
    const Vector<CompositionUnderline>& underlines,
    int selection_start,
    int selection_end) {
  Editor::RevealSelectionScope reveal_selection_scope(&GetEditor());

  GetDocument().UpdateStyleAndLayoutTree();

  SelectComposition();

  if (GetFrame()
          .Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .IsNone())
    return;

  Element* target = GetDocument().FocusedElement();
  if (!target)
    return;

  GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();

  PlainTextRange selected_range = CreateSelectionRangeForSetComposition(
      selection_start, selection_end, text.length());

  if (text.IsEmpty()) {
    if (HasComposition()) {
      Editor::RevealSelectionScope reveal_selection_scope(&GetEditor());
      ReplaceComposition(g_empty_string);
    } else {
      TypingCommand::DeleteSelection(GetDocument(),
                                     TypingCommand::kPreventSpellChecking);
    }

    GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();

    SetEditableSelectionOffsets(selected_range);
    return;
  }

  if (!HasComposition()) {
    target->DispatchEvent(CompositionEvent::Create(
        EventTypeNames::compositionstart, GetFrame().DomWindow(),
        GetFrame().SelectedText()));
    if (!IsAvailable())
      return;
  }

  DCHECK(!text.IsEmpty());

  Clear();

  InsertTextDuringCompositionWithEvents(
      GetFrame(), text,
      TypingCommand::kSelectInsertedText | TypingCommand::kPreventSpellChecking,
      TypingCommand::kTextCompositionUpdate);
  if (!IsAvailable())
    return;

  GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();

  Position base = MostForwardCaretPosition(
      GetFrame().Selection().ComputeVisibleSelectionInDOMTree().Base());
  Node* base_node = base.AnchorNode();
  if (!base_node || !base_node->IsTextNode())
    return;

  Position extent =
      GetFrame().Selection().ComputeVisibleSelectionInDOMTree().Extent();
  Node* extent_node = extent.AnchorNode();

  unsigned extent_offset = extent.ComputeOffsetInContainerNode();
  unsigned base_offset = base.ComputeOffsetInContainerNode();

  has_composition_ = true;
  if (!composition_range_)
    composition_range_ = Range::Create(GetDocument());
  composition_range_->setStart(base_node, base_offset);
  composition_range_->setEnd(extent_node, extent_offset);

  if (base_node->GetLayoutObject())
    base_node->GetLayoutObject()->SetShouldDoFullPaintInvalidation();

  GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();

  SetEditableSelectionOffsets(selected_range, TypingContinuation::kContinue);

  if (underlines.IsEmpty()) {
    GetDocument().Markers().AddCompositionMarker(
        EphemeralRange(composition_range_), Color::kBlack,
        StyleableMarker::Thickness::kThin,
        LayoutTheme::GetTheme().PlatformDefaultCompositionBackgroundColor());
    return;
  }

  const PlainTextRange composition_plain_text_range =
      PlainTextRange::Create(*base_node->parentNode(), *composition_range_);
  AddCompositionUnderlines(underlines, base_node->parentNode(),
                           composition_plain_text_range.Start());
}
