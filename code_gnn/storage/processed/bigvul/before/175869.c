int XfrmController::fillNlAttrXfrmMark(const XfrmId& record, nlattr_xfrm_mark* mark) {
    mark->mark.v = record.mark.v;  
    mark->mark.m = record.mark.m;  
 int len = NLA_HDRLEN + sizeof(xfrm_mark);
    fillXfrmNlaHdr(&mark->hdr, XFRMA_MARK, len);
 return len;
}
