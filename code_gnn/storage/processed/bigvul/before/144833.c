  bool IsTabDiscarded(content::WebContents* content) {
    return TabLifecycleUnitExternal::FromWebContents(content)->IsDiscarded();
  }
