int PluginInstance::NPP_WriteReady(NPStream *stream) {
  DCHECK(npp_functions_ != 0);
  DCHECK(npp_functions_->writeready != 0);
  if (npp_functions_->writeready != 0) {
    return npp_functions_->writeready(npp_, stream);
  }
  return NULL;
}
