  void RunMethod(Method method) {
    (this->*method)();
  }
