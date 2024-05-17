virDomainMemoryPeek(virDomainPtr dom,
                    unsigned long long start  ,
                    size_t size,
                    void *buffer,
                    unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "start=%lld, size=%zi, buffer=%p, flags=%x",
                     start, size, buffer, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckReadOnlyGoto(conn->flags, error);

     

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_MEMORY_VIRTUAL, VIR_MEMORY_PHYSICAL, error);

     
    if (size > 0)
        virCheckNonNullArgGoto(buffer, error);

    if (conn->driver->domainMemoryPeek) {
        int ret;
        ret = conn->driver->domainMemoryPeek(dom, start, size,
                                             buffer, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}
