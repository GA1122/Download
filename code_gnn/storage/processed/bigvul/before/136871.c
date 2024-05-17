HTMLDataListElement* HTMLInputElement::DataList() const {
  if (!has_non_empty_list_)
    return nullptr;

  if (!input_type_->ShouldRespectListAttribute())
    return nullptr;

  return ToHTMLDataListElementOrNull(
      GetTreeScope().getElementById(FastGetAttribute(listAttr)));
}
