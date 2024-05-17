bool TabLifecycleUnitSource::TabLifecycleUnit::Unfreeze() {
  if (!IsValidStateChange(GetState(), LifecycleUnitState::ACTIVE,
                          StateChangeReason::BROWSER_INITIATED)) {
    return false;
  }

  if (GetWebContents()->GetVisibility() == content::Visibility::VISIBLE)
    return false;

  SetState(LifecycleUnitState::ACTIVE, StateChangeReason::BROWSER_INITIATED);
  GetWebContents()->SetPageFrozen(false);
  return true;
}
