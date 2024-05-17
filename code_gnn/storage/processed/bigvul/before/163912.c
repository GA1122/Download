WebString ToString(MeteringMode value) {
  switch (value) {
    case MeteringMode::NONE:
      return WebString::FromUTF8("none");
    case MeteringMode::MANUAL:
      return WebString::FromUTF8("manual");
    case MeteringMode::SINGLE_SHOT:
      return WebString::FromUTF8("single-shot");
    case MeteringMode::CONTINUOUS:
      return WebString::FromUTF8("continuous");
    default:
      NOTREACHED() << "Unknown MeteringMode";
  }
  return WebString();
}
