ukm::SourceId TabLifecycleUnitSource::TabLifecycleUnit::GetUkmSourceId() const {
  resource_coordinator::ResourceCoordinatorTabHelper* observer =
      resource_coordinator::ResourceCoordinatorTabHelper::FromWebContents(
          web_contents());
  if (!observer)
    return ukm::kInvalidSourceId;
  return observer->ukm_source_id();
}
