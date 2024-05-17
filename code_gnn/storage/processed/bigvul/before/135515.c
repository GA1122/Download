void Editor::ApplyParagraphStyle(StylePropertySet* style,
                                 InputEvent::InputType input_type) {
  if (GetFrame()
          .Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .IsNone() ||
      !style)
    return;
  DCHECK(GetFrame().GetDocument());
  ApplyStyleCommand::Create(*GetFrame().GetDocument(),
                            EditingStyle::Create(style), input_type,
                            ApplyStyleCommand::kForceBlockProperties)
      ->Apply();
}
