void fillXfrmNlaHdr(nlattr* hdr, uint16_t type, uint16_t len) {
    hdr->nla_type = type;
    hdr->nla_len = len;
}
