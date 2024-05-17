bool BluetoothSocketAsyncApiFunction::PreRunValidation(std::string* error) {
  if (!UIThreadExtensionFunction::PreRunValidation(error))
    return false;

  if (!BluetoothManifestData::CheckSocketPermitted(extension())) {
    *error = kPermissionDeniedError;
    return false;
  }

  manager_ = ApiResourceManager<BluetoothApiSocket>::Get(browser_context());
  DCHECK(manager_)
      << "There is no socket manager. "
         "If this assertion is failing during a test, then it is likely that "
         "TestExtensionSystem is failing to provide an instance of "
         "ApiResourceManager<BluetoothApiSocket>.";

  if (!manager_) {
    *error = "There is no socket manager.";
    return false;
  }
  return true;
}
