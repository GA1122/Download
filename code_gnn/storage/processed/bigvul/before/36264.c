__u32 sctp_association_get_next_tsn(struct sctp_association *asoc)
{
	 
	__u32 retval = asoc->next_tsn;
	asoc->next_tsn++;
	asoc->unack_data++;

	return retval;
}
