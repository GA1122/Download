netdutils::Status XfrmController::flushPolicyDb(const XfrmSocket& s) {
    std::vector<iovec> iov = {{NULL, 0}};  
 return s.sendMessage(XFRM_MSG_FLUSHPOLICY, NETLINK_REQUEST_FLAGS, 0, &iov);
}
