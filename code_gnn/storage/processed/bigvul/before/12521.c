SPR_ListSuperGroups(struct rx_call *call, afs_int32 aid, prlist *alist,
		    afs_int32 *over)
{
#if defined(SUPERGROUPS)
    afs_int32 code;
    afs_int32 cid = ANONYMOUSID;

    code = listSuperGroups(call, aid, alist, over, &cid);
    osi_auditU(call, PTS_LstSGrps, code, AUD_ID, aid, AUD_END);
    ViceLog(125, ("PTS_ListSuperGroups: code %d cid %d aid %d\n", code, cid, aid));
    return code;
#else
    return RXGEN_OPCODE;
#endif
}
