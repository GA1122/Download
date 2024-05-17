short PluginInstance::NPP_HandleEvent(void* event) {
  DCHECK(npp_functions_ != 0);
  DCHECK(npp_functions_->event != 0);
  if (npp_functions_->event != 0) {
    return npp_functions_->event(npp_, (void*)event);
  }
  return false;
}
