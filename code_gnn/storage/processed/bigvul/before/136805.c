CSSRuleList* LocalDOMWindow::getMatchedCSSRules(
    Element* element,
    const String& pseudo_element) const {
  if (!element)
    return nullptr;

  if (!IsCurrentlyDisplayedInFrame())
    return nullptr;

  unsigned colon_start =
      pseudo_element[0] == ':' ? (pseudo_element[1] == ':' ? 2 : 1) : 0;
  CSSSelector::PseudoType pseudo_type = CSSSelector::ParsePseudoType(
      AtomicString(pseudo_element.Substring(colon_start)), false);
  if (pseudo_type == CSSSelector::kPseudoUnknown && !pseudo_element.IsEmpty())
    return nullptr;

  unsigned rules_to_include = StyleResolver::kAuthorCSSRules;
  PseudoId pseudo_id = CSSSelector::GetPseudoId(pseudo_type);
  element->GetDocument().UpdateStyleAndLayoutTree();
  return document()->EnsureStyleResolver().PseudoCSSRulesForElement(
      element, pseudo_id, rules_to_include);
}
