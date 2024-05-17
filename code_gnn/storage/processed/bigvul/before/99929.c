void NPN_UnscheduleTimer(NPP id, uint32 timer_id) {
  scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
  if (plugin)
    plugin->UnscheduleTimer(timer_id);
}
