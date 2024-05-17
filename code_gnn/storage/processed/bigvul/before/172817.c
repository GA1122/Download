static int open_bluetooth_stack(const struct hw_module_t *module, UNUSED_ATTR char const *name, struct hw_device_t **abstraction) {
 static bluetooth_device_t device = {
 .common = {
 .tag = HARDWARE_DEVICE_TAG,
 .version = 0,
 .close = close_bluetooth_stack,
 },
 .get_bluetooth_interface = bluetooth__get_bluetooth_interface
 };

  device.common.module = (struct hw_module_t *)module;
 *abstraction = (struct hw_device_t *)&device;
 return 0;
}
