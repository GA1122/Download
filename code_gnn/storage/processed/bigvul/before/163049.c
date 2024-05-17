ProfilingProcessHost::~ProfilingProcessHost() {
  if (is_registered_)
    Unregister();
}
