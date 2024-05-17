int HAL_load(void)
{
 int err = 0;

 hw_module_t* module;
 hw_device_t* device;

    bdt_log("Loading HAL lib + extensions");

    err = hw_get_module(BT_HARDWARE_MODULE_ID, (hw_module_t const**)&module);
 if (err == 0)
 {
        err = module->methods->open(module, BT_HARDWARE_MODULE_ID, &device);
 if (err == 0) {
            bt_device = (bluetooth_device_t *)device;
            sBtInterface = bt_device->get_bluetooth_interface();
 }
 }

    bdt_log("HAL library loaded (%s)", strerror(err));

 return err;
}
