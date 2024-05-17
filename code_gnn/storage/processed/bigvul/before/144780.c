bool TabLifecycleUnitSource::TabLifecycleUnit::Freeze() {
  if (!IsValidStateChange(GetState(), LifecycleUnitState::PENDING_FREEZE,
                          StateChangeReason::BROWSER_INITIATED)) {
    return false;
  }

  if (GetWebContents()->GetVisibility() == content::Visibility::VISIBLE)
    return false;

  SetState(LifecycleUnitState::PENDING_FREEZE,
           StateChangeReason::BROWSER_INITIATED);
  GetWebContents()->SetPageFrozen(true);
  return true;
}
