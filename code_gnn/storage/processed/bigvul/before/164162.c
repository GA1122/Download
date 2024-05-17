  void CreateGroupInEmptyOrigin() {

    storage()->LoadOrCreateGroup(kManifestUrl, delegate());
    Verify_CreateGroup();
  }
