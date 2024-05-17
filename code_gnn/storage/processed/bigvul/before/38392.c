static void cm_init_av_for_response(struct cm_port *port, struct ib_wc *wc,
				    struct ib_grh *grh, struct cm_av *av)
{
	av->port = port;
	av->pkey_index = wc->pkey_index;
	ib_init_ah_from_wc(port->cm_dev->ib_device, port->port_num, wc,
 			   grh, &av->ah_attr);
 }