virDomainMigrateVersion2(virDomainPtr domain,
                         virConnectPtr dconn,
                         unsigned long flags,
                         const char *dname,
                         const char *uri,
                         unsigned long bandwidth)
{
    virDomainPtr ddomain = NULL;
    char *uri_out = NULL;
    char *cookie = NULL;
    char *dom_xml = NULL;
    int cookielen = 0, ret;
    virDomainInfo info;
    virErrorPtr orig_err = NULL;
    unsigned int getxml_flags = 0;
    int cancelled;
    unsigned long destflags;

    VIR_DOMAIN_DEBUG(domain,
                     "dconn=%p, flags=%lx, dname=%s, uri=%s, bandwidth=%lu",
                     dconn, flags, NULLSTR(dname), NULLSTR(uri), bandwidth);

     

     
    if (!domain->conn->driver->domainGetXMLDesc) {
        virReportUnsupportedError();
        return NULL;
    }

    if (VIR_DRV_SUPPORTS_FEATURE(domain->conn->driver, domain->conn,
                                 VIR_DRV_FEATURE_XML_MIGRATABLE)) {
        getxml_flags |= VIR_DOMAIN_XML_MIGRATABLE;
    } else {
        getxml_flags |= VIR_DOMAIN_XML_SECURE | VIR_DOMAIN_XML_UPDATE_CPU;
    }

    dom_xml = domain->conn->driver->domainGetXMLDesc(domain, getxml_flags);
    if (!dom_xml)
        return NULL;

    ret = virDomainGetInfo(domain, &info);
    if (ret == 0 && info.state == VIR_DOMAIN_PAUSED)
        flags |= VIR_MIGRATE_PAUSED;

    destflags = flags & ~(VIR_MIGRATE_ABORT_ON_ERROR |
                          VIR_MIGRATE_AUTO_CONVERGE);

    VIR_DEBUG("Prepare2 %p flags=%lx", dconn, destflags);
    ret = dconn->driver->domainMigratePrepare2
        (dconn, &cookie, &cookielen, uri, &uri_out, destflags, dname,
         bandwidth, dom_xml);
    VIR_FREE(dom_xml);
    if (ret == -1)
        goto done;

    if (uri == NULL && uri_out == NULL) {
        virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                       _("domainMigratePrepare2 did not set uri"));
        cancelled = 1;
         
        orig_err = virSaveLastError();
        goto finish;
    }
    if (uri_out)
        uri = uri_out;  

     
    VIR_DEBUG("Perform %p", domain->conn);
    ret = domain->conn->driver->domainMigratePerform
        (domain, cookie, cookielen, uri, flags, dname, bandwidth);

     
    if (ret < 0)
        orig_err = virSaveLastError();

     
    cancelled = ret < 0 ? 1 : 0;

 finish:
     
    dname = dname ? dname : domain->name;
    VIR_DEBUG("Finish2 %p ret=%d", dconn, ret);
    ddomain = dconn->driver->domainMigrateFinish2
        (dconn, dname, cookie, cookielen, uri, destflags, cancelled);
    if (cancelled && ddomain)
        VIR_ERROR(_("finish step ignored that migration was cancelled"));

 done:
    if (orig_err) {
        virSetError(orig_err);
        virFreeError(orig_err);
    }
    VIR_FREE(uri_out);
    VIR_FREE(cookie);
    return ddomain;
}
