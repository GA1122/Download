static int do_xfer(struct xfer_header *xfer)
{
    int r = 0;

    if (xfer->use_replication) {
         
        r = xfer_initialsync(xfer);
        if (r) return r;
    }

    r = xfer_deactivate(xfer);

    if (!r) {
        if (xfer->use_replication) {
             
            r = xfer_finalsync(xfer);
        }
        else {
            r = xfer_localcreate(xfer);
            if (!r) r = xfer_undump(xfer);
        }
    }

    if (r) {
         
        xfer_recover(xfer);
        return r;
    }

     
    xfer_delete(xfer);
    xfer_reactivate(xfer);

    return 0;
}
