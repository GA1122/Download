  static CompositorDependencies& Get() {
    static base::NoDestructor<CompositorDependencies> instance;
    return *instance;
  }
