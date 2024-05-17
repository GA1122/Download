__checkparam_dl(const struct sched_attr *attr)
{
	 
	if (attr->sched_deadline == 0)
		return false;

	 
	if (attr->sched_runtime < (1ULL << DL_SCALE))
		return false;

	 
	if (attr->sched_deadline & (1ULL << 63) ||
	    attr->sched_period & (1ULL << 63))
		return false;

	 
	if ((attr->sched_period != 0 &&
	     attr->sched_period < attr->sched_deadline) ||
	    attr->sched_deadline < attr->sched_runtime)
		return false;

	return true;
}
