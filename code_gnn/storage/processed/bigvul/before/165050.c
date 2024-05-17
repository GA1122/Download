void HTMLCanvasElement::setWidth(unsigned value,
                                 ExceptionState& exception_state) {
  if (IsPlaceholderRegistered()) {
    exception_state.ThrowDOMException(
        DOMExceptionCode::kInvalidStateError,
        "Cannot resize canvas after call to transferControlToOffscreen().");
    return;
  }
  SetUnsignedIntegralAttribute(kWidthAttr, value, kDefaultCanvasWidth);
}
