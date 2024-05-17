uint32 NPN_ScheduleTimer(NPP id,
                         uint32 interval,
                         NPBool repeat,
                         void (*func)(NPP id, uint32 timer_id)) {
  scoped_refptr<NPAPI::PluginInstance> plugin = FindInstance(id);
  if (!plugin)
    return 0;

  return plugin->ScheduleTimer(interval, repeat, func);
}
