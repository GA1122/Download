v8::Extension* LoadTimesExtension::Get() {
  return new LoadTimesExtensionWrapper();
}
