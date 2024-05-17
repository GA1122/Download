void BaseRenderingContext2D::setFilter(
    const ExecutionContext* execution_context,
    const String& filter_string) {
  if (filter_string == GetState().UnparsedFilter())
    return;

  const CSSValue* filter_value = CSSParser::ParseSingleValue(
      CSSPropertyFilter, filter_string,
      CSSParserContext::Create(kHTMLStandardMode,
                               execution_context->GetSecureContextMode()));

  if (!filter_value || filter_value->IsCSSWideKeyword())
    return;

  ModifiableState().SetUnparsedFilter(filter_string);
  ModifiableState().SetFilter(filter_value);
  SnapshotStateForFilter();
}
