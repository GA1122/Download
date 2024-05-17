void PluginHost::PatchNPNetscapeFuncs(NPNetscapeFuncs* overrides) {
  if (overrides->invoke)
    host_funcs_.invoke = overrides->invoke;

  if (overrides->invokeDefault)
    host_funcs_.invokeDefault = overrides->invokeDefault;

  if (overrides->evaluate)
    host_funcs_.evaluate = overrides->evaluate;

  if (overrides->getproperty)
    host_funcs_.getproperty = overrides->getproperty;

  if (overrides->setproperty)
    host_funcs_.setproperty = overrides->setproperty;

  if (overrides->removeproperty)
    host_funcs_.removeproperty = overrides->removeproperty;

  if (overrides->hasproperty)
    host_funcs_.hasproperty = overrides->hasproperty;

  if (overrides->hasmethod)
    host_funcs_.hasmethod = overrides->hasmethod;

  if (overrides->setexception)
    host_funcs_.setexception = overrides->setexception;

  if (overrides->enumerate)
    host_funcs_.enumerate = overrides->enumerate;
}
