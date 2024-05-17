NPError PluginInstance::NPP_SetWindow(NPWindow *window) {
  DCHECK(npp_functions_ != 0);
  DCHECK(npp_functions_->setwindow != 0);

  if (npp_functions_->setwindow != 0) {
    return npp_functions_->setwindow(npp_, window);
  }
  return NPERR_INVALID_FUNCTABLE_ERROR;
}
