NTSTATUS parse_spnego_mechanisms(DATA_BLOB blob_in,
		DATA_BLOB *pblob_out,
		char **kerb_mechOID)
{
	char *OIDs[ASN1_MAX_OIDS];
	int i;
	NTSTATUS ret = NT_STATUS_OK;

	*kerb_mechOID = NULL;

	 
	if (!parse_negTokenTarg(blob_in, OIDs, pblob_out)) {
		return NT_STATUS_LOGON_FAILURE;
	}

	 

#ifdef HAVE_KRB5
	if (strcmp(OID_KERBEROS5, OIDs[0]) == 0 ||
	    strcmp(OID_KERBEROS5_OLD, OIDs[0]) == 0) {
		*kerb_mechOID = SMB_STRDUP(OIDs[0]);
		if (*kerb_mechOID == NULL) {
			ret = NT_STATUS_NO_MEMORY;
		}
	}
#endif

	for (i=0;OIDs[i];i++) {
		DEBUG(5,("parse_spnego_mechanisms: Got OID %s\n", OIDs[i]));
		talloc_free(OIDs[i]);
	}
	return ret;
}
