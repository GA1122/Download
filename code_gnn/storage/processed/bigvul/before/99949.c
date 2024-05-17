NPError PluginInstance::NPP_New(unsigned short mode,
                                short argc,
                                char *argn[],
                                char *argv[]) {
  DCHECK(npp_functions_ != 0);
  DCHECK(npp_functions_->newp != 0);
  DCHECK(argc >= 0);

  if (npp_functions_->newp != 0) {
    return npp_functions_->newp(
        (NPMIMEType)mime_type_.c_str(), npp_, mode,  argc, argn, argv, NULL);
  }
  return NPERR_INVALID_FUNCTABLE_ERROR;
}
