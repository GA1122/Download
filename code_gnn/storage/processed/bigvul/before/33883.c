authenticate_user(const char *user, const char *passwd)
{
#ifndef HAVE_PAM
    gboolean pass = TRUE;
#else
    int rc = 0;
    gboolean pass = FALSE;
    const void *p_user = NULL;

    struct pam_conv p_conv;
    struct pam_handle *pam_h = NULL;
    static const char *pam_name = NULL;

    if (pam_name == NULL) {
        pam_name = getenv("CIB_pam_service");
    }
    if (pam_name == NULL) {
        pam_name = "login";
    }

    p_conv.conv = construct_pam_passwd;
    p_conv.appdata_ptr = strdup(passwd);

    rc = pam_start(pam_name, user, &p_conv, &pam_h);
    if (rc != PAM_SUCCESS) {
        crm_err("Could not initialize PAM: %s (%d)", pam_strerror(pam_h, rc), rc);
        goto bail;
    }

    rc = pam_authenticate(pam_h, 0);
    if (rc != PAM_SUCCESS) {
        crm_err("Authentication failed for %s: %s (%d)", user, pam_strerror(pam_h, rc), rc);
        goto bail;
    }

     
    rc = pam_get_item(pam_h, PAM_USER, &p_user);
    if (rc != PAM_SUCCESS) {
        crm_err("Internal PAM error: %s (%d)", pam_strerror(pam_h, rc), rc);
        goto bail;

    } else if (p_user == NULL) {
        crm_err("Unknown user authenticated.");
        goto bail;

    } else if (safe_str_neq(p_user, user)) {
        crm_err("User mismatch: %s vs. %s.", (const char *)p_user, (const char *)user);
        goto bail;
    }

    rc = pam_acct_mgmt(pam_h, 0);
    if (rc != PAM_SUCCESS) {
        crm_err("Access denied: %s (%d)", pam_strerror(pam_h, rc), rc);
        goto bail;
    }
    pass = TRUE;

  bail:
    rc = pam_end(pam_h, rc);
#endif
    return pass;
}