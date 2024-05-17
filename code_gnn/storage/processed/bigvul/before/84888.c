static int select_sec(struct cifs_ses *ses, struct sess_data *sess_data)
{
	int type;

	type = select_sectype(ses->server, ses->sectype);
	cifs_dbg(FYI, "sess setup type %d\n", type);
	if (type == Unspecified) {
		cifs_dbg(VFS,
			"Unable to select appropriate authentication method!");
		return -EINVAL;
	}

	switch (type) {
	case LANMAN:
		 
#ifdef CONFIG_CIFS_WEAK_PW_HASH
		sess_data->func = sess_auth_lanman;
		break;
#else
		return -EOPNOTSUPP;
#endif
	case NTLM:
		sess_data->func = sess_auth_ntlm;
		break;
	case NTLMv2:
		sess_data->func = sess_auth_ntlmv2;
		break;
	case Kerberos:
#ifdef CONFIG_CIFS_UPCALL
		sess_data->func = sess_auth_kerberos;
		break;
#else
		cifs_dbg(VFS, "Kerberos negotiated but upcall support disabled!\n");
		return -ENOSYS;
		break;
#endif  
	case RawNTLMSSP:
		sess_data->func = sess_auth_rawntlmssp_negotiate;
		break;
	default:
		cifs_dbg(VFS, "secType %d not supported!\n", type);
		return -ENOSYS;
	}

	return 0;
}
