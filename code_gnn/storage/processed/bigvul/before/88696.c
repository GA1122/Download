static void dwc3_prepare_trbs(struct dwc3_ep *dep)
{
	struct dwc3_request	*req, *n;

	BUILD_BUG_ON_NOT_POWER_OF_2(DWC3_TRB_NUM);

	 
	list_for_each_entry(req, &dep->started_list, list) {
		if (req->num_pending_sgs > 0)
			dwc3_prepare_one_trb_sg(dep, req);

		if (!dwc3_calc_trbs_left(dep))
			return;
	}

	list_for_each_entry_safe(req, n, &dep->pending_list, list) {
		struct dwc3	*dwc = dep->dwc;
		int		ret;

		ret = usb_gadget_map_request_by_dev(dwc->sysdev, &req->request,
						    dep->direction);
		if (ret)
			return;

		req->sg			= req->request.sg;
		req->num_pending_sgs	= req->request.num_mapped_sgs;

		if (req->num_pending_sgs > 0)
			dwc3_prepare_one_trb_sg(dep, req);
		else
			dwc3_prepare_one_trb_linear(dep, req);

		if (!dwc3_calc_trbs_left(dep))
			return;
	}
}
