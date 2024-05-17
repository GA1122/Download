NPError PluginInstance::NPP_GetValue(NPPVariable variable, void *value) {
  DCHECK(npp_functions_ != 0);
  if (npp_functions_->getvalue != 0) {
    return npp_functions_->getvalue(npp_, variable, value);
  }
  return NPERR_INVALID_FUNCTABLE_ERROR;
}
