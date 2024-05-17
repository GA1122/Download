NPError PluginInstance::NPP_NewStream(NPMIMEType type,
                                      NPStream *stream,
                                      NPBool seekable,
                                      unsigned short *stype) {
  DCHECK(npp_functions_ != 0);
  DCHECK(npp_functions_->newstream != 0);
  if (npp_functions_->newstream != 0) {
      return npp_functions_->newstream(npp_, type, stream, seekable, stype);
  }
  return NPERR_INVALID_FUNCTABLE_ERROR;
}
