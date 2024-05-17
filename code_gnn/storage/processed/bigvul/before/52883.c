static int ib_uverbs_cleanup_ucontext(struct ib_uverbs_file *file,
				      struct ib_ucontext *context)
{
	struct ib_uobject *uobj, *tmp;

	context->closing = 1;

	list_for_each_entry_safe(uobj, tmp, &context->ah_list, list) {
		struct ib_ah *ah = uobj->object;

		idr_remove_uobj(&ib_uverbs_ah_idr, uobj);
		ib_destroy_ah(ah);
		kfree(uobj);
	}

	 
	list_for_each_entry_safe(uobj, tmp, &context->mw_list, list) {
		struct ib_mw *mw = uobj->object;

		idr_remove_uobj(&ib_uverbs_mw_idr, uobj);
		uverbs_dealloc_mw(mw);
		kfree(uobj);
	}

	list_for_each_entry_safe(uobj, tmp, &context->rule_list, list) {
		struct ib_flow *flow_id = uobj->object;

		idr_remove_uobj(&ib_uverbs_rule_idr, uobj);
		ib_destroy_flow(flow_id);
		kfree(uobj);
	}

	list_for_each_entry_safe(uobj, tmp, &context->qp_list, list) {
		struct ib_qp *qp = uobj->object;
		struct ib_uqp_object *uqp =
			container_of(uobj, struct ib_uqp_object, uevent.uobject);

		idr_remove_uobj(&ib_uverbs_qp_idr, uobj);
		if (qp != qp->real_qp) {
			ib_close_qp(qp);
		} else {
			ib_uverbs_detach_umcast(qp, uqp);
			ib_destroy_qp(qp);
		}
		ib_uverbs_release_uevent(file, &uqp->uevent);
		kfree(uqp);
	}

	list_for_each_entry_safe(uobj, tmp, &context->srq_list, list) {
		struct ib_srq *srq = uobj->object;
		struct ib_uevent_object *uevent =
			container_of(uobj, struct ib_uevent_object, uobject);

		idr_remove_uobj(&ib_uverbs_srq_idr, uobj);
		ib_destroy_srq(srq);
		ib_uverbs_release_uevent(file, uevent);
		kfree(uevent);
	}

	list_for_each_entry_safe(uobj, tmp, &context->cq_list, list) {
		struct ib_cq *cq = uobj->object;
		struct ib_uverbs_event_file *ev_file = cq->cq_context;
		struct ib_ucq_object *ucq =
			container_of(uobj, struct ib_ucq_object, uobject);

		idr_remove_uobj(&ib_uverbs_cq_idr, uobj);
		ib_destroy_cq(cq);
		ib_uverbs_release_ucq(file, ev_file, ucq);
		kfree(ucq);
	}

	list_for_each_entry_safe(uobj, tmp, &context->mr_list, list) {
		struct ib_mr *mr = uobj->object;

		idr_remove_uobj(&ib_uverbs_mr_idr, uobj);
		ib_dereg_mr(mr);
		kfree(uobj);
	}

	mutex_lock(&file->device->xrcd_tree_mutex);
	list_for_each_entry_safe(uobj, tmp, &context->xrcd_list, list) {
		struct ib_xrcd *xrcd = uobj->object;
		struct ib_uxrcd_object *uxrcd =
			container_of(uobj, struct ib_uxrcd_object, uobject);

		idr_remove_uobj(&ib_uverbs_xrcd_idr, uobj);
		ib_uverbs_dealloc_xrcd(file->device, xrcd);
		kfree(uxrcd);
	}
	mutex_unlock(&file->device->xrcd_tree_mutex);

	list_for_each_entry_safe(uobj, tmp, &context->pd_list, list) {
		struct ib_pd *pd = uobj->object;

		idr_remove_uobj(&ib_uverbs_pd_idr, uobj);
		ib_dealloc_pd(pd);
		kfree(uobj);
	}

	put_pid(context->tgid);

	return context->device->dealloc_ucontext(context);
}