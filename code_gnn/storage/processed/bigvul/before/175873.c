int XfrmController::fillUserSaId(const XfrmId& record, xfrm_usersa_id* said) {
    said->daddr = record.dstAddr;
    said->spi = record.spi;
    said->family = record.addrFamily;
    said->proto = IPPROTO_ESP;

 return sizeof(*said);
}
