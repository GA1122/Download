AviaryScheddPlugin::markDirty(const char *key,
							const char *name,
							const char *value)
{
	if (!IS_JOB(key)) return;
	if (!(strcasecmp(name, ATTR_JOB_STATUS) == 0 ||
		  strcasecmp(name, ATTR_LAST_JOB_STATUS) == 0)) return;

	DirtyJobStatus status(name, atoi(value));
	DirtyJobEntry entry(key, status);
	dirtyJobs->push_back(DirtyJobEntry(key, DirtyJobStatus(name, atoi(value))));

	if (!isHandlerRegistered) {
		daemonCore->Register_Timer(0,
								   (TimerHandlercpp)
								   &AviaryScheddPlugin::processDirtyJobs,
								   "Process Dirty",
								   this);
		isHandlerRegistered = true;
	}
}
