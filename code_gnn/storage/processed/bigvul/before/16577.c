FileTransfer::FileTransfer()
{
	TransferFilePermissions = false;
	DelegateX509Credentials = false;
	PeerDoesTransferAck = false;
	PeerDoesGoAhead = false;
	PeerUnderstandsMkdir = false;
	TransferUserLog = false;
	Iwd = NULL;
	ExceptionFiles = NULL;
	InputFiles = NULL;
	OutputFiles = NULL;
	EncryptInputFiles = NULL;
	EncryptOutputFiles = NULL;
	DontEncryptInputFiles = NULL;
	DontEncryptOutputFiles = NULL;
	IntermediateFiles = NULL;
	SpooledIntermediateFiles = NULL;
	FilesToSend = NULL;
	EncryptFiles = NULL;
	DontEncryptFiles = NULL;
	OutputDestination = NULL;
	ExecFile = NULL;
	UserLogFile = NULL;
	X509UserProxy = NULL;
	TransSock = NULL;
	TransKey = NULL;
	SpoolSpace = NULL;
	TmpSpoolSpace = NULL;
	user_supplied_key = FALSE;
	upload_changed_files = false;
	last_download_catalog = NULL;
	last_download_time = 0;
	ActiveTransferTid = -1;
	TransferStart = 0;
	ClientCallback = 0;
	ClientCallbackClass = NULL;
	TransferPipe[0] = TransferPipe[1] = -1;
	bytesSent = 0.0;
	bytesRcvd = 0.0;
	m_final_transfer_flag = FALSE;
#ifdef WIN32
	perm_obj = NULL;
#endif
	desired_priv_state = PRIV_UNKNOWN;
	want_priv_change = false;
	did_init = false;
	clientSockTimeout = 30;
	simple_init = true;
	simple_sock = NULL;
	m_use_file_catalog = true;
	m_sec_session_id = NULL;
	I_support_filetransfer_plugins = false;
	plugin_table = NULL;
}
