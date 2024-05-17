 static void qp_event_handler(struct ib_event *event, void *context)
 {
	struct svc_xprt *xprt = context;

	switch (event->event) {
	 
	case IB_EVENT_PATH_MIG:
	case IB_EVENT_COMM_EST:
	case IB_EVENT_SQ_DRAINED:
	case IB_EVENT_QP_LAST_WQE_REACHED:
		dprintk("svcrdma: QP event %s (%d) received for QP=%p\n",
			ib_event_msg(event->event), event->event,
			event->element.qp);
		break;
	 
	case IB_EVENT_PATH_MIG_ERR:
	case IB_EVENT_QP_FATAL:
	case IB_EVENT_QP_REQ_ERR:
	case IB_EVENT_QP_ACCESS_ERR:
	case IB_EVENT_DEVICE_FATAL:
	default:
		dprintk("svcrdma: QP ERROR event %s (%d) received for QP=%p, "
			"closing transport\n",
			ib_event_msg(event->event), event->event,
			event->element.qp);
		set_bit(XPT_CLOSE, &xprt->xpt_flags);
		break;
	}
}