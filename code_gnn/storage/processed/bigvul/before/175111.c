 virtual int create() {
 int nlctrlFamily = genl_ctrl_resolve(mInfo->cmd_sock, "nlctrl");
 int ret = mMsg.create(nlctrlFamily, CTRL_CMD_GETFAMILY, 0, 0);
 if (ret < 0) {
 return ret;
 }
        ret = mMsg.put_string(CTRL_ATTR_FAMILY_NAME, mName);
 return ret;
 }
