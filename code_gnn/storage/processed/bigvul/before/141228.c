void Document::SetFindInPageRoot(Element* find_in_page_root) {
  DCHECK(RuntimeEnabledFeatures::InvisibleDOMEnabled());
  DCHECK(!find_in_page_root || !find_in_page_root_);
  find_in_page_root_ = find_in_page_root;
}
