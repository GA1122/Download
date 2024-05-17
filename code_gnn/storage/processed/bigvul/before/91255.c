static int intf_err_seq(struct ipmi_smi *intf,
			long         msgid,
			unsigned int err)
{
	int                  rv = -ENODEV;
	unsigned long        flags;
	unsigned char        seq;
	unsigned long        seqid;
	struct ipmi_recv_msg *msg = NULL;


	GET_SEQ_FROM_MSGID(msgid, seq, seqid);

	spin_lock_irqsave(&intf->seq_lock, flags);
	 
	if ((intf->seq_table[seq].inuse)
				&& (intf->seq_table[seq].seqid == seqid)) {
		struct seq_table *ent = &intf->seq_table[seq];

		ent->inuse = 0;
		msg = ent->recv_msg;
		rv = 0;
	}
	spin_unlock_irqrestore(&intf->seq_lock, flags);

	if (msg)
		deliver_err_response(intf, msg, err);

	return rv;
}