SVGElement::~SVGElement() {
  DCHECK(isConnected() || !HasRelativeLengths());
}
