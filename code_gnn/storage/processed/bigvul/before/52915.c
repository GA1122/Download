static int disarm_req_delay(struct qib_ctxtdata *rcd)
{
	int ret = 0;

	if (!usable(rcd->ppd)) {
		int i;
		 
		if (rcd->user_event_mask) {
			 
			set_bit(_QIB_EVENT_DISARM_BUFS_BIT,
				&rcd->user_event_mask[0]);
			for (i = 1; i < rcd->subctxt_cnt; i++)
				set_bit(_QIB_EVENT_DISARM_BUFS_BIT,
					&rcd->user_event_mask[i]);
		}
		for (i = 0; !usable(rcd->ppd) && i < 300; i++)
			msleep(100);
		ret = -ENETDOWN;
	}
	return ret;
}
