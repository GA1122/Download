static void dwc3_prepare_one_trb_sg(struct dwc3_ep *dep,
		struct dwc3_request *req)
{
	struct scatterlist *sg = req->sg;
	struct scatterlist *s;
	int		i;

	for_each_sg(sg, s, req->num_pending_sgs, i) {
		unsigned int length = req->request.length;
		unsigned int maxp = usb_endpoint_maxp(dep->endpoint.desc);
		unsigned int rem = length % maxp;
		unsigned chain = true;

		if (sg_is_last(s))
			chain = false;

		if (rem && usb_endpoint_dir_out(dep->endpoint.desc) && !chain) {
			struct dwc3	*dwc = dep->dwc;
			struct dwc3_trb	*trb;

			req->unaligned = true;

			 
			dwc3_prepare_one_trb(dep, req, true, i);

			 
			trb = &dep->trb_pool[dep->trb_enqueue];
			__dwc3_prepare_one_trb(dep, trb, dwc->bounce_addr,
					maxp - rem, false, 0,
					req->request.stream_id,
					req->request.short_not_ok,
					req->request.no_interrupt);
		} else {
			dwc3_prepare_one_trb(dep, req, chain, i);
		}

		if (!dwc3_calc_trbs_left(dep))
			break;
	}
}