OomInterventionImpl::OomInterventionImpl()
    : timer_(Platform::Current()->MainThread()->GetTaskRunner(),
             this,
             &OomInterventionImpl::Check) {}
