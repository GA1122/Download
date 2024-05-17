int XfrmController::fillNlAttrUserTemplate(const XfrmSaInfo& record, nlattr_user_tmpl* tmpl) {
    fillUserTemplate(record, &tmpl->tmpl);

 int len = NLA_HDRLEN + sizeof(xfrm_user_tmpl);
    fillXfrmNlaHdr(&tmpl->hdr, XFRMA_TMPL, len);
 return len;
}
