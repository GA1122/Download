static void spice_server_char_device_remove_interface(SpiceBaseInstance *sin)
{
    SpiceCharDeviceInstance* char_device =
            SPICE_CONTAINEROF(sin, SpiceCharDeviceInstance, base);

    spice_info("remove CHAR_DEVICE %s", char_device->subtype);
    if (strcmp(char_device->subtype, SUBTYPE_VDAGENT) == 0) {
        if (vdagent) {
            reds_agent_remove();
        }
    }
#ifdef USE_SMARTCARD
    else if (strcmp(char_device->subtype, SUBTYPE_SMARTCARD) == 0) {
        smartcard_device_disconnect(char_device);
    }
#endif
    else if (strcmp(char_device->subtype, SUBTYPE_USBREDIR) == 0 ||
             strcmp(char_device->subtype, SUBTYPE_PORT) == 0) {
        spicevmc_device_disconnect(char_device);
    } else {
        spice_warning("failed to remove char device %s", char_device->subtype);
    }

    char_device->st = NULL;
}
