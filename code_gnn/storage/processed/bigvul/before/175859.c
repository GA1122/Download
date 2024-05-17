netdutils::Status XfrmController::deleteSecurityAssociation(const XfrmId& record,
 const XfrmSocket& sock) {
    xfrm_usersa_id said{};
    nlattr_xfrm_mark xfrmmark{};

 enum { NLMSG_HDR, USERSAID, USERSAID_PAD, MARK, MARK_PAD };

    std::vector<iovec> iov = {
 {NULL, 0},  
 {&said, 0},  
 {kPadBytes, 0},  
 {&xfrmmark, 0},  
 {kPadBytes, 0},  
 };

 int len;
    len = iov[USERSAID].iov_len = fillUserSaId(record, &said);
    iov[USERSAID_PAD].iov_len = NLMSG_ALIGN(len) - len;

    len = iov[MARK].iov_len = fillNlAttrXfrmMark(record, &xfrmmark);
    iov[MARK_PAD].iov_len = NLA_ALIGN(len) - len;

 return sock.sendMessage(XFRM_MSG_DELSA, NETLINK_REQUEST_FLAGS, 0, &iov);
}
