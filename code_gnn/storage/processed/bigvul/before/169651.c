  void SimulateFreezeSignal(content::WebContents* contents) {
    TabLifecycleUnitSource::GetInstance()
        ->GetTabLifecycleUnit(contents)
        ->UpdateLifecycleState(mojom::LifecycleState::kFrozen);
  }
