static int llc_sap_next_state(struct llc_sap *sap, struct sk_buff *skb)
{
	int rc = 1;
	struct llc_sap_state_trans *trans;

	if (sap->state > LLC_NR_SAP_STATES)
		goto out;
	trans = llc_find_sap_trans(sap, skb);
	if (!trans)
		goto out;
	 
	rc = llc_exec_sap_trans_actions(sap, trans, skb);
	if (rc)
		goto out;
	 
	sap->state = trans->next_state;
out:
	return rc;
}