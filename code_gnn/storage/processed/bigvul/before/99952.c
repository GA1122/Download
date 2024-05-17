NPError PluginInstance::NPP_SetValue(NPNVariable variable, void *value) {
  DCHECK(npp_functions_ != 0);
  if (npp_functions_->setvalue != 0) {
    return npp_functions_->setvalue(npp_, variable, value);
  }
  return NPERR_INVALID_FUNCTABLE_ERROR;
}
