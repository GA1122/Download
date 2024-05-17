netdutils::Status XfrmController::deleteTunnelModeSecurityPolicy(const XfrmSaInfo& record,
 const XfrmSocket& sock,
 XfrmDirection direction) {
    xfrm_userpolicy_id policyid{};
    nlattr_xfrm_mark xfrmmark{};

 enum {
        NLMSG_HDR,
        USERPOLICYID,
        USERPOLICYID_PAD,
        MARK,
        MARK_PAD,
 };

    std::vector<iovec> iov = {
 {NULL, 0},  
 {&policyid, 0},  
 {kPadBytes, 0},  
 {&xfrmmark, 0},  
 {kPadBytes, 0},  
 };

 int len = iov[USERPOLICYID].iov_len = fillUserPolicyId(record, direction, &policyid);
    iov[USERPOLICYID_PAD].iov_len = NLMSG_ALIGN(len) - len;

    len = iov[MARK].iov_len = fillNlAttrXfrmMark(record, &xfrmmark);
    iov[MARK_PAD].iov_len = NLA_ALIGN(len) - len;

 return sock.sendMessage(XFRM_MSG_DELPOLICY, NETLINK_REQUEST_FLAGS, 0, &iov);
}
