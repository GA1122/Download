void HTMLMediaElement::SetSrc(const USVStringOrTrustedURL& stringOrURL,
                              ExceptionState& exception_state) {
  setAttribute(srcAttr, stringOrURL, exception_state);
}
