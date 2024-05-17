static bool blk_pm_allow_request(struct request *rq)
{
	switch (rq->q->rpm_status) {
	case RPM_RESUMING:
	case RPM_SUSPENDING:
		return rq->rq_flags & RQF_PM;
	case RPM_SUSPENDED:
		return false;
	default:
		return true;
	}
}
