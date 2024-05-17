virDomainMigrateVersion3Full(virDomainPtr domain,
                             virConnectPtr dconn,
                             const char *xmlin,
                             const char *dname,
                             const char *uri,
                             unsigned long long bandwidth,
                             virTypedParameterPtr params,
                             int nparams,
                             bool useParams,
                             unsigned int flags)
{
    virDomainPtr ddomain = NULL;
    char *uri_out = NULL;
    char *cookiein = NULL;
    char *cookieout = NULL;
    char *dom_xml = NULL;
    int cookieinlen = 0;
    int cookieoutlen = 0;
    int ret;
    virDomainInfo info;
    virErrorPtr orig_err = NULL;
    int cancelled = 1;
    unsigned long protection = 0;
    bool notify_source = true;
    unsigned int destflags;
    int state;
    virTypedParameterPtr tmp;

    VIR_DOMAIN_DEBUG(domain,
                     "dconn=%p, xmlin=%s, dname=%s, uri=%s, bandwidth=%llu, "
                     "params=%p, nparams=%d, useParams=%d, flags=%x",
                     dconn, NULLSTR(xmlin), NULLSTR(dname), NULLSTR(uri),
                     bandwidth, params, nparams, useParams, flags);
    VIR_TYPED_PARAMS_DEBUG(params, nparams);

    if ((!useParams &&
         (!domain->conn->driver->domainMigrateBegin3 ||
          !domain->conn->driver->domainMigratePerform3 ||
          !domain->conn->driver->domainMigrateConfirm3 ||
          !dconn->driver->domainMigratePrepare3 ||
          !dconn->driver->domainMigrateFinish3)) ||
        (useParams &&
         (!domain->conn->driver->domainMigrateBegin3Params ||
          !domain->conn->driver->domainMigratePerform3Params ||
          !domain->conn->driver->domainMigrateConfirm3Params ||
          !dconn->driver->domainMigratePrepare3Params ||
          !dconn->driver->domainMigrateFinish3Params))) {
        virReportUnsupportedError();
        return NULL;
    }

    if (virTypedParamsCopy(&tmp, params, nparams) < 0)
        return NULL;
    params = tmp;

    if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                 VIR_DRV_FEATURE_MIGRATE_CHANGE_PROTECTION))
        protection = VIR_MIGRATE_CHANGE_PROTECTION;

    VIR_DEBUG("Begin3 %p", domain->conn);
    if (useParams) {
        dom_xml = domain->conn->driver->domainMigrateBegin3Params
            (domain, params, nparams, &cookieout, &cookieoutlen,
             flags | protection);
    } else {
        dom_xml = domain->conn->driver->domainMigrateBegin3
            (domain, xmlin, &cookieout, &cookieoutlen,
             flags | protection, dname, bandwidth);
    }
    if (!dom_xml)
        goto done;

    if (useParams) {
         
        ret = virDomainGetState(domain, &state, NULL, 0);
    } else {
        ret = virDomainGetInfo(domain, &info);
        state = info.state;
    }
    if (ret == 0 && state == VIR_DOMAIN_PAUSED)
        flags |= VIR_MIGRATE_PAUSED;

    destflags = flags & ~(VIR_MIGRATE_ABORT_ON_ERROR |
                          VIR_MIGRATE_AUTO_CONVERGE);

    VIR_DEBUG("Prepare3 %p flags=%x", dconn, destflags);
    cookiein = cookieout;
    cookieinlen = cookieoutlen;
    cookieout = NULL;
    cookieoutlen = 0;
    if (useParams) {
        if (virTypedParamsReplaceString(&params, &nparams,
                                        VIR_MIGRATE_PARAM_DEST_XML,
                                        dom_xml) < 0)
            goto done;
        ret = dconn->driver->domainMigratePrepare3Params
            (dconn, params, nparams, cookiein, cookieinlen,
             &cookieout, &cookieoutlen, &uri_out, destflags);
    } else {
        ret = dconn->driver->domainMigratePrepare3
            (dconn, cookiein, cookieinlen, &cookieout, &cookieoutlen,
             uri, &uri_out, destflags, dname, bandwidth, dom_xml);
    }
    if (ret == -1) {
        if (protection) {
             
            orig_err = virSaveLastError();
            goto confirm;
        } else {
            goto done;
        }
    }

     
    if (uri_out) {
        uri = uri_out;
        if (useParams &&
            virTypedParamsReplaceString(&params, &nparams,
                                        VIR_MIGRATE_PARAM_URI,
                                        uri_out) < 0) {
            cancelled = 1;
            orig_err = virSaveLastError();
            goto finish;
        }
    } else if (!uri &&
               virTypedParamsGetString(params, nparams,
                                       VIR_MIGRATE_PARAM_URI, &uri) <= 0) {
        virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                       _("domainMigratePrepare3 did not set uri"));
        cancelled = 1;
        orig_err = virSaveLastError();
        goto finish;
    }

    if (flags & VIR_MIGRATE_OFFLINE) {
        VIR_DEBUG("Offline migration, skipping Perform phase");
        VIR_FREE(cookieout);
        cookieoutlen = 0;
        cancelled = 0;
        goto finish;
    }

     
    VIR_DEBUG("Perform3 %p uri=%s", domain->conn, uri);
    VIR_FREE(cookiein);
    cookiein = cookieout;
    cookieinlen = cookieoutlen;
    cookieout = NULL;
    cookieoutlen = 0;
     
    if (useParams) {
        ret = domain->conn->driver->domainMigratePerform3Params
            (domain, NULL, params, nparams, cookiein, cookieinlen,
             &cookieout, &cookieoutlen, flags | protection);
    } else {
        ret = domain->conn->driver->domainMigratePerform3
            (domain, NULL, cookiein, cookieinlen,
             &cookieout, &cookieoutlen, NULL,
             uri, flags | protection, dname, bandwidth);
    }

     
    if (ret < 0) {
        orig_err = virSaveLastError();
         
        notify_source = false;
    }

     
    cancelled = ret < 0 ? 1 : 0;

 finish:
     
    VIR_DEBUG("Finish3 %p ret=%d", dconn, ret);
    VIR_FREE(cookiein);
    cookiein = cookieout;
    cookieinlen = cookieoutlen;
    cookieout = NULL;
    cookieoutlen = 0;
    if (useParams) {
        if (virTypedParamsGetString(params, nparams,
                                    VIR_MIGRATE_PARAM_DEST_NAME, NULL) <= 0 &&
            virTypedParamsReplaceString(&params, &nparams,
                                        VIR_MIGRATE_PARAM_DEST_NAME,
                                        domain->name) < 0) {
            ddomain = NULL;
        } else {
            ddomain = dconn->driver->domainMigrateFinish3Params
                (dconn, params, nparams, cookiein, cookieinlen,
                 &cookieout, &cookieoutlen, destflags, cancelled);
        }
    } else {
        dname = dname ? dname : domain->name;
        ddomain = dconn->driver->domainMigrateFinish3
            (dconn, dname, cookiein, cookieinlen, &cookieout, &cookieoutlen,
             NULL, uri, destflags, cancelled);
    }

    if (cancelled) {
        if (ddomain) {
            VIR_ERROR(_("finish step ignored that migration was cancelled"));
        } else {
             
            if (orig_err &&
                orig_err->domain == VIR_FROM_QEMU &&
                orig_err->code == VIR_ERR_OPERATION_FAILED) {
                virErrorPtr err = virGetLastError();
                if (err &&
                    err->domain == VIR_FROM_QEMU &&
                    err->code != VIR_ERR_MIGRATE_FINISH_OK) {
                    virFreeError(orig_err);
                    orig_err = NULL;
                }
            }
        }
    }

     
    cancelled = ddomain == NULL ? 1 : 0;

     
    if (!orig_err)
        orig_err = virSaveLastError();

 confirm:
     
    if (notify_source) {
        VIR_DEBUG("Confirm3 %p ret=%d domain=%p", domain->conn, ret, domain);
        VIR_FREE(cookiein);
        cookiein = cookieout;
        cookieinlen = cookieoutlen;
        cookieout = NULL;
        cookieoutlen = 0;
        if (useParams) {
            ret = domain->conn->driver->domainMigrateConfirm3Params
                (domain, params, nparams, cookiein, cookieinlen,
                 flags | protection, cancelled);
        } else {
            ret = domain->conn->driver->domainMigrateConfirm3
                (domain, cookiein, cookieinlen,
                 flags | protection, cancelled);
        }
         
        if (ret < 0) {
            VIR_WARN("Guest %s probably left in 'paused' state on source",
                     domain->name);
        }
    }

 done:
    if (orig_err) {
        virSetError(orig_err);
        virFreeError(orig_err);
    }
    VIR_FREE(dom_xml);
    VIR_FREE(uri_out);
    VIR_FREE(cookiein);
    VIR_FREE(cookieout);
    virTypedParamsFree(params, nparams);
    return ddomain;
}
