void SVGElement::ChildrenChanged(const ChildrenChange& change) {
  Element::ChildrenChanged(change);

  InvalidateInstances();
}
