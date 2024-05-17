static const AtomicString UniqueClassnameAmongSiblings(Element* element) {
  DCHECK(element);

  auto classname_filter = std::make_unique<ClassnameFilter>();

  Element* parent_element = ElementTraversal::FirstAncestor(*element);
  Element* sibling_element =
      parent_element ? ElementTraversal::FirstChild(*parent_element) : element;
  for (; sibling_element;
       sibling_element = ElementTraversal::NextSibling(*sibling_element)) {
    if (sibling_element->HasClass() && sibling_element != element) {
      const SpaceSplitString& class_names = sibling_element->ClassNames();
      for (wtf_size_t i = 0; i < class_names.size(); ++i) {
        classname_filter->Add(class_names[i]);
      }
    }
  }

  const SpaceSplitString& class_names = element->ClassNames();
  for (wtf_size_t i = 0; i < class_names.size(); ++i) {
    if (!classname_filter->MayContain(class_names[i])) {
      return class_names[i];
    }
  }

  return AtomicString();
}
