static void __dwc3_gadget_start_isoc(struct dwc3 *dwc,
		struct dwc3_ep *dep, u32 cur_uf)
{
	if (list_empty(&dep->pending_list)) {
		dev_info(dwc->dev, "%s: ran out of requests\n",
				dep->name);
		dep->flags |= DWC3_EP_PENDING_REQUEST;
		return;
	}

	 
	dep->frame_number = cur_uf + max_t(u32, 4, dep->interval);
	__dwc3_gadget_kick_transfer(dep);
}