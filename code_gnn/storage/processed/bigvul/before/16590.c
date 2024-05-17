FileTransfer::ObtainAndSendTransferGoAhead(DCTransferQueue &xfer_queue,bool downloading,Stream *s,char const *full_fname,bool &go_ahead_always)
{
	bool result;
	bool try_again = true;
	int hold_code = 0;
	int hold_subcode = 0;
	MyString error_desc;

	result = DoObtainAndSendTransferGoAhead(xfer_queue,downloading,s,full_fname,go_ahead_always,try_again,hold_code,hold_subcode,error_desc);

	if( !result ) {
		SaveTransferInfo(false,try_again,hold_code,hold_subcode,error_desc.Value());
		if( error_desc.Length() ) {
			dprintf(D_ALWAYS,"%s\n",error_desc.Value());
		}
	}
	return result;
}
