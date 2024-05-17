select_sectype(struct TCP_Server_Info *server, enum securityEnum requested)
{
	switch (server->negflavor) {
	case CIFS_NEGFLAVOR_EXTENDED:
		switch (requested) {
		case Kerberos:
		case RawNTLMSSP:
			return requested;
		case Unspecified:
			if (server->sec_ntlmssp &&
			    (global_secflags & CIFSSEC_MAY_NTLMSSP))
				return RawNTLMSSP;
			if ((server->sec_kerberos || server->sec_mskerberos) &&
			    (global_secflags & CIFSSEC_MAY_KRB5))
				return Kerberos;
			 
		default:
			return Unspecified;
		}
	case CIFS_NEGFLAVOR_UNENCAP:
		switch (requested) {
		case NTLM:
		case NTLMv2:
			return requested;
		case Unspecified:
			if (global_secflags & CIFSSEC_MAY_NTLMV2)
				return NTLMv2;
			if (global_secflags & CIFSSEC_MAY_NTLM)
				return NTLM;
		default:
			 
			break;
		}
	case CIFS_NEGFLAVOR_LANMAN:
		switch (requested) {
		case LANMAN:
			return requested;
		case Unspecified:
			if (global_secflags & CIFSSEC_MAY_LANMAN)
				return LANMAN;
			 
		default:
			return Unspecified;
		}
	default:
		return Unspecified;
	}
}