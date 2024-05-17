void ib_uverbs_qp_event_handler(struct ib_event *event, void *context_ptr)
{
	struct ib_uevent_object *uobj;

	 
	if (!event->element.qp->uobject || !event->element.qp->uobject->live)
		return;

	uobj = container_of(event->element.qp->uobject,
			    struct ib_uevent_object, uobject);

	ib_uverbs_async_handler(context_ptr, uobj->uobject.user_handle,
				event->event, &uobj->event_list,
				&uobj->events_reported);
}