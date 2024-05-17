PP_Resource PPB_URLLoader_Impl::Create(PP_Instance pp_instance) {
  PluginInstance* instance = ResourceTracker::Get()->GetInstance(pp_instance);
  if (!instance)
    return 0;

  PPB_URLLoader_Impl* loader = new PPB_URLLoader_Impl(instance, false);
  return loader->GetReference();
}
