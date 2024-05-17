std::string RetrievalMethodToString(
    const WebElementDescriptor::RetrievalMethod& method) {
  switch (method) {
    case WebElementDescriptor::CSS_SELECTOR:
      return "CSS_SELECTOR";
    case WebElementDescriptor::ID:
      return "ID";
    case WebElementDescriptor::NONE:
      return "NONE";
  }
  NOTREACHED();
  return "UNKNOWN";
}
