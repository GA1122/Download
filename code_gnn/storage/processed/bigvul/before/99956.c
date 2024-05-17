int PluginInstance::NPP_Write(NPStream *stream,
                              int offset,
                              int len,
                              void *buffer) {
  DCHECK(npp_functions_ != 0);
  DCHECK(npp_functions_->write != 0);
  if (npp_functions_->write != 0) {
    return npp_functions_->write(npp_, stream, offset, len, buffer);
  }
  return NULL;
}
