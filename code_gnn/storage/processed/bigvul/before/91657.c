void bgp_packet_mpattr_prefix(struct stream *s, afi_t afi, safi_t safi,
			      struct prefix *p, struct prefix_rd *prd,
			      mpls_label_t *label, uint32_t num_labels,
			      int addpath_encode, uint32_t addpath_tx_id,
			      struct attr *attr)
{
	if (safi == SAFI_MPLS_VPN) {
		if (addpath_encode)
			stream_putl(s, addpath_tx_id);
		 
		stream_putc(s, p->prefixlen + 88);
		stream_put(s, label, BGP_LABEL_BYTES);
		stream_put(s, prd->val, 8);
		stream_put(s, &p->u.prefix, PSIZE(p->prefixlen));
	} else if (afi == AFI_L2VPN && safi == SAFI_EVPN) {
		 
		bgp_evpn_encode_prefix(s, p, prd, label, num_labels, attr,
				       addpath_encode, addpath_tx_id);
	} else if (safi == SAFI_LABELED_UNICAST) {
		 
		stream_put_labeled_prefix(s, p, label);
	} else if (safi == SAFI_FLOWSPEC) {
		if (PSIZE (p->prefixlen)+2 < FLOWSPEC_NLRI_SIZELIMIT)
			stream_putc(s, PSIZE (p->prefixlen)+2);
		else
			stream_putw(s, (PSIZE (p->prefixlen)+2)|(0xf<<12));
		stream_putc(s, 2); 
		stream_putc(s, p->prefixlen); 
		stream_put(s, &p->u.prefix, PSIZE (p->prefixlen));
	} else
		stream_put_prefix_addpath(s, p, addpath_encode, addpath_tx_id);
}