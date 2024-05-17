void HTMLFormElement::DidAssociateByParser() {
  if (!did_finish_parsing_children_)
    return;
  has_elements_associated_by_parser_ = true;
  UseCounter::Count(GetDocument(), WebFeature::kFormAssociationByParser);
}
