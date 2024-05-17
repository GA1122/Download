bool PluginInstance::NPP_Print(NPPrint* platform_print) {
  DCHECK(npp_functions_ != 0);
  if (npp_functions_->print != 0) {
    npp_functions_->print(npp_, platform_print);
    return true;
  }
  return false;
}
