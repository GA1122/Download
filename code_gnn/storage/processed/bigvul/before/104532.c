  void RegisterFunction() {
    factories_[T::function_name()] = &NewExtensionFunction<T>;
  }
