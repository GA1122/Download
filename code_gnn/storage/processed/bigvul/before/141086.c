static HTMLLinkElement* GetLinkElement(const Document* doc,
                                       bool (*match_fn)(HTMLLinkElement&)) {
  HTMLHeadElement* head = doc->head();
  if (!head)
    return nullptr;

  for (HTMLLinkElement& link_element :
       Traversal<HTMLLinkElement>::ChildrenOf(*head)) {
    if (match_fn(link_element))
      return &link_element;
  }
  return nullptr;
}
