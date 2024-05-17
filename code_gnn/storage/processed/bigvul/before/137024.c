void HTMLInputElement::setSize(unsigned size, ExceptionState& exception_state) {
  if (size == 0) {
    exception_state.ThrowDOMException(
        kIndexSizeError, "The value provided is 0, which is an invalid size.");
  } else {
    SetUnsignedIntegralAttribute(sizeAttr, size ? size : kDefaultSize,
                                 kDefaultSize);
  }
}
