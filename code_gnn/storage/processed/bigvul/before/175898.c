 static netdutils::Status validateResponse(NetlinkResponse response, size_t len) {
 if (len < sizeof(nlmsghdr)) {
            ALOGW("Invalid response message received over netlink");
 return netdutils::statusFromErrno(EBADMSG, "Invalid message");
 }

 switch (response.hdr.nlmsg_type) {
 case NLMSG_NOOP:
 case NLMSG_DONE:
 return netdutils::status::ok;
 case NLMSG_OVERRUN:
                ALOGD("Netlink request overran kernel buffer");
 return netdutils::statusFromErrno(EBADMSG, "Kernel buffer overrun");
 case NLMSG_ERROR:
 if (len < sizeof(NetlinkResponse::_err_)) {
                    ALOGD("Netlink message received malformed error response");
 return netdutils::statusFromErrno(EBADMSG, "Malformed error response");
 }
 return netdutils::statusFromErrno(
 -response.err.err.error,
 "Error netlink message");  
 case XFRM_MSG_NEWSA:
 break;
 }

 if (response.hdr.nlmsg_type < XFRM_MSG_BASE   ||
            response.hdr.nlmsg_type > XFRM_MSG_MAX) {
            ALOGD("Netlink message responded with an out-of-range message ID");
 return netdutils::statusFromErrno(EBADMSG, "Invalid message ID");
 }

 return netdutils::status::ok;
 }
