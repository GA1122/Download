const AttrNameToTrustedType& HTMLMediaElement::GetCheckedAttributeTypes()
    const {
  DEFINE_STATIC_LOCAL(AttrNameToTrustedType, attribute_map,
                      ({{"src", SpecificTrustedType::kTrustedURL}}));
  return attribute_map;
}
