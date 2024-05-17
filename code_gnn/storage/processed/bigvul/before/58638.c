BOOL rdp_recv_set_error_info_data_pdu(rdpRdp* rdp, STREAM* s)
{
	if (stream_get_left(s) < 4)
		return FALSE;

	stream_read_UINT32(s, rdp->errorInfo);  

	if (rdp->errorInfo != ERRINFO_SUCCESS)
		rdp_print_errinfo(rdp->errorInfo);

	return TRUE;
}