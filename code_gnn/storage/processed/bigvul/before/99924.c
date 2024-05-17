NPError NPN_RequestRead(NPStream* stream, NPByteRange* range_list) {
  if (!stream || !range_list)
    return NPERR_GENERIC_ERROR;

  scoped_refptr<NPAPI::PluginInstance> plugin =
      reinterpret_cast<NPAPI::PluginInstance*>(stream->ndata);
  if (!plugin.get())
    return NPERR_GENERIC_ERROR;

  plugin->RequestRead(stream, range_list);
   return NPERR_NO_ERROR;
 }
