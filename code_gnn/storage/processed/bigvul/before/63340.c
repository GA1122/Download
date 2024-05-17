val_inq_ctx_args(
    OM_uint32 *minor_status,
    gss_ctx_id_t context_handle,
    gss_name_t *src_name,
    gss_name_t *targ_name,
    OM_uint32 *lifetime_rec,
    gss_OID *mech_type,
    OM_uint32 *ctx_flags,
    int *locally_initiated,
    int *opened)
{

     

    if (minor_status != NULL)
	*minor_status = 0;

    if (src_name != NULL)
	*src_name = GSS_C_NO_NAME;

    if (targ_name != NULL)
	*targ_name = GSS_C_NO_NAME;

    if (mech_type != NULL)
	*mech_type = GSS_C_NO_OID;

     

    if (minor_status == NULL)
	return (GSS_S_CALL_INACCESSIBLE_WRITE);

    if (context_handle == GSS_C_NO_CONTEXT)
	return (GSS_S_CALL_INACCESSIBLE_READ | GSS_S_NO_CONTEXT);

    return (GSS_S_COMPLETE);
}