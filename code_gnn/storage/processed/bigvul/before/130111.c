  scoped_ptr<MetricsStateManager> CreateStateManager() {
    return MetricsStateManager::Create(
        &prefs_,
        base::Bind(&MetricsStateManagerTest::is_metrics_reporting_enabled,
                   base::Unretained(this)),
        base::Bind(&MetricsStateManagerTest::MockStoreClientInfoBackup,
                   base::Unretained(this)),
        base::Bind(&MetricsStateManagerTest::LoadFakeClientInfoBackup,
                   base::Unretained(this))).Pass();
  }
