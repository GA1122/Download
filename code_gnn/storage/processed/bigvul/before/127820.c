PepperMediaDeviceManager::~PepperMediaDeviceManager() {
  DCHECK(enumerate_callbacks_.empty());
  DCHECK(open_callbacks_.empty());
}
