static int dwc3_gadget_init_endpoints(struct dwc3 *dwc, u8 total)
{
	struct dwc3_ep			*dep;
	u8				epnum;

	INIT_LIST_HEAD(&dwc->gadget.ep_list);

	for (epnum = 0; epnum < total; epnum++) {
		bool			direction = epnum & 1;
		u8			num = epnum >> 1;

		dep = kzalloc(sizeof(*dep), GFP_KERNEL);
		if (!dep)
			return -ENOMEM;

		dep->dwc = dwc;
		dep->number = epnum;
		dep->direction = direction;
		dep->regs = dwc->regs + DWC3_DEP_BASE(epnum);
		dwc->eps[epnum] = dep;

		snprintf(dep->name, sizeof(dep->name), "ep%u%s", num,
				direction ? "in" : "out");

		dep->endpoint.name = dep->name;

		if (!(dep->number > 1)) {
			dep->endpoint.desc = &dwc3_gadget_ep0_desc;
			dep->endpoint.comp_desc = NULL;
		}

		spin_lock_init(&dep->lock);

		if (num == 0) {
			usb_ep_set_maxpacket_limit(&dep->endpoint, 512);
			dep->endpoint.maxburst = 1;
			dep->endpoint.ops = &dwc3_gadget_ep0_ops;
			if (!direction)
				dwc->gadget.ep0 = &dep->endpoint;
		} else if (direction) {
			int mdwidth;
			int kbytes;
			int size;
			int ret;

			mdwidth = DWC3_MDWIDTH(dwc->hwparams.hwparams0);
			 
			mdwidth /= 8;

			size = dwc3_readl(dwc->regs, DWC3_GTXFIFOSIZ(num));
			if (dwc3_is_usb31(dwc))
				size = DWC31_GTXFIFOSIZ_TXFDEF(size);
			else
				size = DWC3_GTXFIFOSIZ_TXFDEF(size);

			 
			size *= mdwidth;

			kbytes = size / 1024;
			if (kbytes == 0)
				kbytes = 1;

			 
			size -= mdwidth * (kbytes + 1);
			size /= kbytes;

			usb_ep_set_maxpacket_limit(&dep->endpoint, size);

			dep->endpoint.max_streams = 15;
			dep->endpoint.ops = &dwc3_gadget_ep_ops;
			list_add_tail(&dep->endpoint.ep_list,
					&dwc->gadget.ep_list);

			ret = dwc3_alloc_trb_pool(dep);
			if (ret)
				return ret;
		} else {
			int		ret;

			usb_ep_set_maxpacket_limit(&dep->endpoint, 1024);
			dep->endpoint.max_streams = 15;
			dep->endpoint.ops = &dwc3_gadget_ep_ops;
			list_add_tail(&dep->endpoint.ep_list,
					&dwc->gadget.ep_list);

			ret = dwc3_alloc_trb_pool(dep);
			if (ret)
				return ret;
		}

		if (num == 0) {
			dep->endpoint.caps.type_control = true;
		} else {
			dep->endpoint.caps.type_iso = true;
			dep->endpoint.caps.type_bulk = true;
			dep->endpoint.caps.type_int = true;
		}

		dep->endpoint.caps.dir_in = direction;
		dep->endpoint.caps.dir_out = !direction;

		INIT_LIST_HEAD(&dep->pending_list);
		INIT_LIST_HEAD(&dep->started_list);
	}

	return 0;
}