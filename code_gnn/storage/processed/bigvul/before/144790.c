content::Visibility TabLifecycleUnitSource::TabLifecycleUnit::GetVisibility()
    const {
  return GetWebContents()->GetVisibility();
}
