  static Type* New() {
    if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kTestType))
      return DefaultSingletonTraits<TestType>::New();
    else
      return DefaultSingletonTraits<Type>::New();
  }
