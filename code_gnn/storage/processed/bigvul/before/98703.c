NPError WebPluginDelegatePepper::Device3DSynchronizeContext(
    NPP id,
    NPDeviceContext3D* context,
    NPDeviceSynchronizationMode mode,
    const int32* input_attrib_list,
    int32* output_attrib_list,
    NPDeviceSynchronizeContextCallbackPtr callback,
    void* callback_data) {
  if (!context)
    return NPERR_GENERIC_ERROR;

  if (input_attrib_list) {
    for (const int32* attrib_pair = input_attrib_list;
         *attrib_pair;
         attrib_pair += 2) {
      switch (attrib_pair[0]) {
        case NP3DAttrib_PutOffset:
          context->putOffset = attrib_pair[1];
          break;
        default:
          return NPERR_GENERIC_ERROR;
      }
    }
  }

  if (mode != NPDeviceSynchronizationMode_Cached) {
    context->waitForProgress = mode == NPDeviceSynchronizationMode_Flush;
    Device3DFlushContext(id, context, callback, callback_data);
  }

  if (output_attrib_list) {
    for (int32* attrib_pair = output_attrib_list;
         *attrib_pair;
         attrib_pair += 2) {
      switch (attrib_pair[0]) {
        case NP3DAttrib_CommandBufferSize:
          attrib_pair[1] = context->commandBufferSize;
          break;
        case NP3DAttrib_GetOffset:
          attrib_pair[1] = context->getOffset;
          break;
        case NP3DAttrib_PutOffset:
          attrib_pair[1] = context->putOffset;
          break;
        case NP3DAttrib_Token:
          attrib_pair[1] = context->token;
          break;
        default:
          return NPERR_GENERIC_ERROR;
      }
    }
  }

  return NPERR_NO_ERROR;
}
