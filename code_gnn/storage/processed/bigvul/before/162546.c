static const char* InitiatorTypeNameToString(
    const AtomicString& initiator_type_name) {
  if (initiator_type_name == FetchInitiatorTypeNames::css)
    return "CSS resource";
  if (initiator_type_name == FetchInitiatorTypeNames::document)
    return "Document";
  if (initiator_type_name == FetchInitiatorTypeNames::icon)
    return "Icon";
  if (initiator_type_name == FetchInitiatorTypeNames::internal)
    return "Internal resource";
  if (initiator_type_name == FetchInitiatorTypeNames::link)
    return "Link element resource";
  if (initiator_type_name == FetchInitiatorTypeNames::processinginstruction)
    return "Processing instruction";
  if (initiator_type_name == FetchInitiatorTypeNames::texttrack)
    return "Text track";
  if (initiator_type_name == FetchInitiatorTypeNames::uacss)
    return "User Agent CSS resource";
  if (initiator_type_name == FetchInitiatorTypeNames::xml)
    return "XML resource";
  if (initiator_type_name == FetchInitiatorTypeNames::xmlhttprequest)
    return "XMLHttpRequest";

  static_assert(
      FetchInitiatorTypeNames::FetchInitiatorTypeNamesCount == 13,
      "New FetchInitiatorTypeNames should be handled correctly here.");

  return "Resource";
}
