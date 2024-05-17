void HTMLMediaElement::SetSrc(const USVStringOrTrustedURL& stringOrURL,
                              ExceptionState& exception_state) {
  setAttribute(kSrcAttr, stringOrURL, exception_state);
}
