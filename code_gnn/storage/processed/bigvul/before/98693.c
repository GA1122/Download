NPError WebPluginDelegatePepper::Device3DCreateContext(
    int32 config,
    int32* attrib_list,
    NPDeviceContext3D** context) {
  if (!context)
    return NPERR_GENERIC_ERROR;

  if (config != 0)
    return NPERR_GENERIC_ERROR;

  NPDeviceContext3DConfig old_config;
  old_config.commandBufferSize = kDefaultCommandBufferSize;
  if (attrib_list) {
    for (int32* attrib_pair = attrib_list; *attrib_pair; attrib_pair += 2) {
      switch (attrib_pair[0]) {
        case NP3DAttrib_CommandBufferSize:
          old_config.commandBufferSize = attrib_pair[1];
          break;
        default:
          return NPERR_GENERIC_ERROR;
      }
    }
  }

  *context = new NPDeviceContext3D;
  Device3DInitializeContext(&old_config, *context);

  Device3DImpl* impl = static_cast<Device3DImpl*>((*context)->reserved);
  impl->dynamically_created = true;

  return NPERR_NO_ERROR;
}
