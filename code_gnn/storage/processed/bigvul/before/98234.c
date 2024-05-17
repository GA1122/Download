static jboolean JNI_TestDummySupport_OpenAndVerifyNativeLibrary(JNIEnv* env) {
#if BUILDFLAG(USE_NATIVE_MODULES)
  void* handle =
      base::android::BundleUtils::DlOpenModuleLibraryPartition("test_dummy");
  if (handle == nullptr) {
    LOG(ERROR) << "Cannot open test library: " << dlerror();
    return false;
  }

  void* symbol = dlsym(handle, "TestDummyEntrypoint");
  if (symbol == nullptr) {
    LOG(ERROR) << "Cannot find test library symbol";
    return false;
  }

  typedef int TestFunction();
  TestFunction* test_function = reinterpret_cast<TestFunction*>(symbol);
  if (test_function() != 123) {
    LOG(ERROR) << "Unexpected value from test library";
    return false;
  }
#endif

  return true;
}
