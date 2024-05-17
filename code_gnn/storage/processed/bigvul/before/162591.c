Resource::~Resource() {
  InstanceCounters::DecrementCounter(InstanceCounters::kResourceCounter);
}
