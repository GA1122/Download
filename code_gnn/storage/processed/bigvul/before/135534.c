void Editor::ComputeAndSetTypingStyle(StylePropertySet* style,
                                      InputEvent::InputType input_type) {
  if (!style || style->IsEmpty()) {
    ClearTypingStyle();
    return;
  }

  if (typing_style_)
    typing_style_->OverrideWithStyle(style);
  else
    typing_style_ = EditingStyle::Create(style);

  typing_style_->PrepareToApplyAt(
      GetFrame()
          .Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .VisibleStart()
          .DeepEquivalent(),
      EditingStyle::kPreserveWritingDirection);

  EditingStyle* block_style = typing_style_->ExtractAndRemoveBlockProperties();
  if (!block_style->IsEmpty()) {
    DCHECK(GetFrame().GetDocument());
    ApplyStyleCommand::Create(*GetFrame().GetDocument(), block_style,
                              input_type)
        ->Apply();
  }
}
