FileTransfer::SaveTransferInfo(bool success,bool try_again,int hold_code,int hold_subcode,char const *hold_reason)
{
	Info.success = success;
	Info.try_again = try_again;
	Info.hold_code = hold_code;
	Info.hold_subcode = hold_subcode;
	if( hold_reason ) {
		Info.error_desc = hold_reason;
	}
}
