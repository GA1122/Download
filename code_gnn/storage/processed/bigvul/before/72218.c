fmt_intarg(ServerOpCodes code, int val)
{
	if (val == -1)
		return "unset";
	switch (code) {
	case sAddressFamily:
		return fmt_multistate_int(val, multistate_addressfamily);
	case sPermitRootLogin:
		return fmt_multistate_int(val, multistate_permitrootlogin);
	case sGatewayPorts:
		return fmt_multistate_int(val, multistate_gatewayports);
	case sCompression:
		return fmt_multistate_int(val, multistate_compression);
	case sUsePrivilegeSeparation:
		return fmt_multistate_int(val, multistate_privsep);
	case sAllowTcpForwarding:
		return fmt_multistate_int(val, multistate_tcpfwd);
	case sAllowStreamLocalForwarding:
		return fmt_multistate_int(val, multistate_tcpfwd);
	case sFingerprintHash:
		return ssh_digest_alg_name(val);
	default:
		switch (val) {
		case 0:
			return "no";
		case 1:
			return "yes";
		default:
			return "UNKNOWN";
		}
	}
}
