static int llc_find_offset(int state, int ev_type)
{
	int rc = 0;
	 
	switch (ev_type) {
	case LLC_CONN_EV_TYPE_PRIM:
		rc = llc_offset_table[state][0]; break;
	case LLC_CONN_EV_TYPE_PDU:
		rc = llc_offset_table[state][4]; break;
	case LLC_CONN_EV_TYPE_SIMPLE:
		rc = llc_offset_table[state][1]; break;
	case LLC_CONN_EV_TYPE_P_TMR:
	case LLC_CONN_EV_TYPE_ACK_TMR:
	case LLC_CONN_EV_TYPE_REJ_TMR:
	case LLC_CONN_EV_TYPE_BUSY_TMR:
		rc = llc_offset_table[state][3]; break;
	}
	return rc;
}