static void uwbd_event_handle(struct uwb_event *evt)
{
	struct uwb_rc *rc;
	int should_keep;

	rc = evt->rc;

	if (rc->ready) {
		switch (evt->type) {
		case UWB_EVT_TYPE_NOTIF:
			should_keep = uwbd_event_handle_urc(evt);
			if (should_keep <= 0)
				kfree(evt->notif.rceb);
			break;
		case UWB_EVT_TYPE_MSG:
			uwbd_event_handle_message(evt);
			break;
		default:
			dev_err(&rc->uwb_dev.dev, "UWBD: invalid event type %d\n", evt->type);
			break;
		}
	}

	__uwb_rc_put(rc);	 
}