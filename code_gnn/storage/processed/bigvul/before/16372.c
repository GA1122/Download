pseudo_job_termination( ClassAd *ad )
{
	bool exited_by_signal = false;
	bool core_dumped = false;
	int exit_signal = 0;
	int exit_code = 0;
	MyString exit_reason;

	ad->LookupBool(ATTR_ON_EXIT_BY_SIGNAL,exited_by_signal);
	ad->LookupBool(ATTR_JOB_CORE_DUMPED,core_dumped);
	ad->LookupString(ATTR_EXIT_REASON,exit_reason);

	ad->LookupInteger(ATTR_ON_EXIT_SIGNAL,exit_signal);
	ad->LookupInteger(ATTR_ON_EXIT_CODE,exit_code);

	Shadow->mockTerminateJob( exit_reason, exited_by_signal, exit_code,
		exit_signal, core_dumped );

	return 0;
}
