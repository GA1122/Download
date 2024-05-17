  static CrostiniUpgraderFactory* GetInstance() {
    static base::NoDestructor<CrostiniUpgraderFactory> factory;
    return factory.get();
  }
