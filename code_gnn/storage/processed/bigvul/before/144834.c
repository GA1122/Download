  bool IsTabFrozen(content::WebContents* content) {
    const LifecycleUnitState state =
        static_cast<TabLifecycleUnitSource::TabLifecycleUnit*>(
            TabLifecycleUnitExternal::FromWebContents(content))
            ->GetState();
    return state == LifecycleUnitState::PENDING_FREEZE ||
           state == LifecycleUnitState::FROZEN;
  }
