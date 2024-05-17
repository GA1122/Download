    TabSpecificContentSettings::BlockedResourcesForType(
        ContentSettingsType content_type) const {
  if (blocked_resources_[content_type].get()) {
    return *blocked_resources_[content_type];
  } else {
    CR_DEFINE_STATIC_LOCAL(std::set<std::string>, empty_set, ());
    return empty_set;
  }
}
