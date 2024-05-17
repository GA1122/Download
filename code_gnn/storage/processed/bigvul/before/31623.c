sctp_disposition_t sctp_sf_ignore_primitive(
	struct net *net,
	const struct sctp_endpoint *ep,
	const struct sctp_association *asoc,
	const sctp_subtype_t type,
	void *arg,
	sctp_cmd_seq_t *commands)
{
	SCTP_DEBUG_PRINTK("Primitive type %d is ignored.\n", type.primitive);
	return SCTP_DISPOSITION_DISCARD;
}