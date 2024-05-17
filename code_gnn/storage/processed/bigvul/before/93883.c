virDomainMigrateVersion1(virDomainPtr domain,
                         virConnectPtr dconn,
                         unsigned long flags,
                         const char *dname,
                         const char *uri,
                         unsigned long bandwidth)
{
    virDomainPtr ddomain = NULL;
    char *uri_out = NULL;
    char *cookie = NULL;
    int cookielen = 0, ret;
    virDomainInfo info;
    unsigned int destflags;

    VIR_DOMAIN_DEBUG(domain,
                     "dconn=%p, flags=%lx, dname=%s, uri=%s, bandwidth=%lu",
                     dconn, flags, NULLSTR(dname), NULLSTR(uri), bandwidth);

    ret = virDomainGetInfo(domain, &info);
    if (ret == 0 && info.state == VIR_DOMAIN_PAUSED)
        flags |= VIR_MIGRATE_PAUSED;

    destflags = flags & ~(VIR_MIGRATE_ABORT_ON_ERROR |
                          VIR_MIGRATE_AUTO_CONVERGE);

     
    if (dconn->driver->domainMigratePrepare
        (dconn, &cookie, &cookielen, uri, &uri_out, destflags, dname,
         bandwidth) == -1)
        goto done;

    if (uri == NULL && uri_out == NULL) {
        virReportError(VIR_ERR_INTERNAL_ERROR, "%s",
                       _("domainMigratePrepare did not set uri"));
        goto done;
    }
    if (uri_out)
        uri = uri_out;  

     
    if (domain->conn->driver->domainMigratePerform
        (domain, cookie, cookielen, uri, flags, dname, bandwidth) == -1)
        goto done;

     
    dname = dname ? dname : domain->name;
    if (dconn->driver->domainMigrateFinish)
        ddomain = dconn->driver->domainMigrateFinish
            (dconn, dname, cookie, cookielen, uri, destflags);
    else
        ddomain = virDomainLookupByName(dconn, dname);

 done:
    VIR_FREE(uri_out);
    VIR_FREE(cookie);
    return ddomain;
}
