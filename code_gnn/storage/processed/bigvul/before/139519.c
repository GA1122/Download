static bool ExecuteToggleStyleInList(LocalFrame& frame,
                                     EditorCommandSource source,
                                     InputEvent::InputType input_type,
                                     CSSPropertyID property_id,
                                     CSSValue* value) {
  EditingStyle* selection_style =
      EditingStyleUtilities::CreateStyleAtSelectionStart(
          frame.Selection().ComputeVisibleSelectionInDOMTreeDeprecated());
  if (!selection_style || !selection_style->Style())
    return false;

  const CSSValue* selected_css_value =
      selection_style->Style()->GetPropertyCSSValue(property_id);
  String new_style("none");
  if (selected_css_value->IsValueList()) {
    CSSValueList* selected_css_value_list =
        ToCSSValueList(selected_css_value)->Copy();
    if (!selected_css_value_list->RemoveAll(*value))
      selected_css_value_list->Append(*value);
    if (selected_css_value_list->length())
      new_style = selected_css_value_list->CssText();

  } else if (selected_css_value->CssText() == "none") {
    new_style = value->CssText();
  }

  MutableStylePropertySet* new_mutable_style =
      MutableStylePropertySet::Create(kHTMLQuirksMode);
  new_mutable_style->SetProperty(property_id, new_style);
  return ApplyCommandToFrame(frame, source, input_type, new_mutable_style);
}
