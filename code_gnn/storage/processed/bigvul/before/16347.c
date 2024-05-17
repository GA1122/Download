BaseShadow::resourceBeganExecution( RemoteResource*   )
{
	began_execution = true;

	shadow_user_policy.startTimer();
		
	startQueueUpdateTimer();

	int job_start_cnt = 0;
	jobAd->LookupInteger(ATTR_NUM_JOB_STARTS, job_start_cnt);
	job_start_cnt++;
	jobAd->Assign(ATTR_NUM_JOB_STARTS, job_start_cnt);
	dprintf(D_FULLDEBUG, "Set %s to %d\n", ATTR_NUM_JOB_STARTS, job_start_cnt);

	if (m_lazy_queue_update) {
		job_updater->watchAttribute(ATTR_NUM_JOB_STARTS);
	}
	else {
		updateJobAttr(ATTR_NUM_JOB_STARTS, job_start_cnt);
	}
}
