SYSCALL_DEFINE2(osf_getrusage, int, who, struct rusage32 __user *, ru)
{
	struct rusage32 r;

	if (who != RUSAGE_SELF && who != RUSAGE_CHILDREN)
		return -EINVAL;

	memset(&r, 0, sizeof(r));
	switch (who) {
	case RUSAGE_SELF:
		jiffies_to_timeval32(current->utime, &r.ru_utime);
		jiffies_to_timeval32(current->stime, &r.ru_stime);
		r.ru_minflt = current->min_flt;
		r.ru_majflt = current->maj_flt;
		break;
	case RUSAGE_CHILDREN:
		jiffies_to_timeval32(current->signal->cutime, &r.ru_utime);
		jiffies_to_timeval32(current->signal->cstime, &r.ru_stime);
		r.ru_minflt = current->signal->cmin_flt;
		r.ru_majflt = current->signal->cmaj_flt;
		break;
	}

	return copy_to_user(ru, &r, sizeof(r)) ? -EFAULT : 0;
}
