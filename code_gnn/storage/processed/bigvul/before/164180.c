  void LoadGroupAndCache_FarHit() {
    PushNextTask(
        base::BindOnce(&AppCacheStorageImplTest::Verify_LoadCache_Far_Hit,
                       base::Unretained(this)));

    MakeCacheAndGroup(kManifestUrl, 1, 1, true);
    group_ = nullptr;
    cache_ = nullptr;

    storage()->LoadCache(1, delegate());
  }
