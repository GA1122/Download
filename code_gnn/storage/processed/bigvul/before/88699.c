static void dwc3_remove_requests(struct dwc3 *dwc, struct dwc3_ep *dep)
{
	struct dwc3_request		*req;

	dwc3_stop_active_transfer(dwc, dep->number, true);

	 
	while (!list_empty(&dep->started_list)) {
		req = next_request(&dep->started_list);

		dwc3_gadget_giveback(dep, req, -ESHUTDOWN);
	}

	while (!list_empty(&dep->pending_list)) {
		req = next_request(&dep->pending_list);

		dwc3_gadget_giveback(dep, req, -ESHUTDOWN);
	}
}