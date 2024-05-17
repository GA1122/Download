static int ap_init_queue(ap_qid_t qid)
{
	struct ap_queue_status status;
	int rc, dummy, i;

	rc = -ENODEV;
	status = ap_reset_queue(qid);
	for (i = 0; i < AP_MAX_RESET; i++) {
		switch (status.response_code) {
		case AP_RESPONSE_NORMAL:
			if (status.queue_empty)
				rc = 0;
			break;
		case AP_RESPONSE_Q_NOT_AVAIL:
		case AP_RESPONSE_DECONFIGURED:
		case AP_RESPONSE_CHECKSTOPPED:
			i = AP_MAX_RESET;	 
			break;
		case AP_RESPONSE_RESET_IN_PROGRESS:
			rc = -EBUSY;
		case AP_RESPONSE_BUSY:
		default:
			break;
		}
		if (rc != -ENODEV && rc != -EBUSY)
			break;
		if (i < AP_MAX_RESET - 1) {
			 
			schedule_timeout(AP_RESET_TIMEOUT);
			status = ap_test_queue(qid, &dummy, &dummy);
		}
	}
	if (rc == 0 && ap_using_interrupts()) {
		rc = ap_queue_enable_interruption(qid, ap_airq.lsi_ptr);
		 
		if (rc)
			pr_err("Registering adapter interrupts for "
			       "AP %d failed\n", AP_QID_DEVICE(qid));
	}
	return rc;
}
