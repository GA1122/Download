allow_mech_by_default(gss_OID mech)
{
    OM_uint32 status, minor;
    gss_OID_set attrs;
    int reject = 0, p;

     
    mech = gssint_get_public_oid(mech);
    if (mech == GSS_C_NO_OID)
	return 0;

    status = gss_inquire_attrs_for_mech(&minor, mech, &attrs, NULL);
    if (status)
	return 0;

     
    if (generic_gss_test_oid_set_member(&minor, GSS_C_MA_DEPRECATED,
					attrs, &p) != GSS_S_COMPLETE || p)
	reject = 1;
    else if (generic_gss_test_oid_set_member(&minor, GSS_C_MA_NOT_DFLT_MECH,
					     attrs, &p) != GSS_S_COMPLETE || p)
	reject = 1;

    (void) gss_release_oid_set(&minor, &attrs);
    return !reject;
}