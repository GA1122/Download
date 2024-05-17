static bool is_gss_cred(struct svc_cred *cr)
{
	 
	return (cr->cr_flavor > RPC_AUTH_MAXFLAVOR);
}
