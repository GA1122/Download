int XfrmController::fillNlAttrXfrmEncapTmpl(const XfrmSaInfo& record, nlattr_encap_tmpl* tmpl) {
 if (record.encap.type == XfrmEncapType::NONE) {
 return 0;
 }

 int len = NLA_HDRLEN + sizeof(xfrm_encap_tmpl);
    tmpl->tmpl.encap_type = static_cast<uint16_t>(record.encap.type);
    tmpl->tmpl.encap_sport = htons(record.encap.srcPort);
    tmpl->tmpl.encap_dport = htons(record.encap.dstPort);
    fillXfrmNlaHdr(&tmpl->hdr, XFRMA_ENCAP, len);
 return len;
}
