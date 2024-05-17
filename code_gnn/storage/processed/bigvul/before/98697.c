NPError WebPluginDelegatePepper::Device3DGetConfigAttribs(
    int32 config,
    int32* attrib_list) {
  if (config != 0)
    return NPERR_GENERIC_ERROR;

  if (attrib_list) {
    for (int32* attrib_pair = attrib_list; *attrib_pair; attrib_pair += 2) {
      switch (attrib_pair[0]) {
        case NP3DAttrib_BufferSize:
          attrib_pair[1] = 32;
          break;
        case NP3DAttrib_AlphaSize:
        case NP3DAttrib_BlueSize:
        case NP3DAttrib_GreenSize:
        case NP3DAttrib_RedSize:
          attrib_pair[1] = 8;
          break;
        case NP3DAttrib_DepthSize:
          attrib_pair[1] = 24;
          break;
        case NP3DAttrib_StencilSize:
          attrib_pair[1] = 8;
          break;
        case NP3DAttrib_SurfaceType:
          attrib_pair[1] = 0;
          break;
        default:
          return NPERR_GENERIC_ERROR;
      }
    }
  }

  return NPERR_NO_ERROR;
}
