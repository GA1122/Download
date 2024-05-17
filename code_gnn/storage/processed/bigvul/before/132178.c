NOINLINE void BadCastCrashIntentionally() {
  class A {
    virtual void f() {}
  };

  class B {
    virtual void f() {}
  };

  A a;
  (void)(B*)&a;
}
