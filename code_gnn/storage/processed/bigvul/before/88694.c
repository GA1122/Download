static void dwc3_prepare_one_trb_linear(struct dwc3_ep *dep,
		struct dwc3_request *req)
{
	unsigned int length = req->request.length;
	unsigned int maxp = usb_endpoint_maxp(dep->endpoint.desc);
	unsigned int rem = length % maxp;

	if (rem && usb_endpoint_dir_out(dep->endpoint.desc)) {
		struct dwc3	*dwc = dep->dwc;
		struct dwc3_trb	*trb;

		req->unaligned = true;

		 
		dwc3_prepare_one_trb(dep, req, true, 0);

		 
		trb = &dep->trb_pool[dep->trb_enqueue];
		__dwc3_prepare_one_trb(dep, trb, dwc->bounce_addr, maxp - rem,
				false, 0, req->request.stream_id,
				req->request.short_not_ok,
				req->request.no_interrupt);
	} else if (req->request.zero && req->request.length &&
		   (IS_ALIGNED(req->request.length,dep->endpoint.maxpacket))) {
		struct dwc3	*dwc = dep->dwc;
		struct dwc3_trb	*trb;

		req->zero = true;

		 
		dwc3_prepare_one_trb(dep, req, true, 0);

		 
		trb = &dep->trb_pool[dep->trb_enqueue];
		__dwc3_prepare_one_trb(dep, trb, dwc->bounce_addr, 0,
				false, 0, req->request.stream_id,
				req->request.short_not_ok,
				req->request.no_interrupt);
	} else {
		dwc3_prepare_one_trb(dep, req, false, 0);
	}
}