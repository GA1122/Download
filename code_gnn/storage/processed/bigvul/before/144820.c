  LifecycleUnit* GetLifecycleUnitAt(int index) {
    return static_cast<TabLifecycleUnitSource::TabLifecycleUnit*>(
        TabLifecycleUnitExternal::FromWebContents(GetWebContentsAt(index)));
  }
