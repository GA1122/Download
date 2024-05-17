bool TabLifecycleUnitSource::TabLifecycleUnit::Load() {
  if (GetLoadingState() != LifecycleUnitLoadingState::UNLOADED)
    return false;


  GetWebContents()->GetController().SetNeedsReload();
  GetWebContents()->GetController().LoadIfNecessary();
  return true;
}
