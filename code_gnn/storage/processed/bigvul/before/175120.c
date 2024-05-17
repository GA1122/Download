 virtual int handleResponse(WifiEvent& reply) {


 struct nlattr **tb = reply.attributes();
 struct genlmsghdr *gnlh = reply.header();
 struct nlattr *mcgrp = NULL;
 int i;

 if (!tb[CTRL_ATTR_MCAST_GROUPS]) {
            ALOGI("No multicast groups found");
 return NL_SKIP;
 } else {
 }

        for_each_attr(mcgrp, tb[CTRL_ATTR_MCAST_GROUPS], i) {

 struct nlattr *tb2[CTRL_ATTR_MCAST_GRP_MAX + 1];
            nla_parse(tb2, CTRL_ATTR_MCAST_GRP_MAX, (nlattr *)nla_data(mcgrp),
                nla_len(mcgrp), NULL);
 if (!tb2[CTRL_ATTR_MCAST_GRP_NAME] || !tb2[CTRL_ATTR_MCAST_GRP_ID]) {
 continue;
 }

 char *grpName = (char *)nla_data(tb2[CTRL_ATTR_MCAST_GRP_NAME]);
 int grpNameLen = nla_len(tb2[CTRL_ATTR_MCAST_GRP_NAME]);


 if (strncmp(grpName, mGroup, grpNameLen) != 0)
 continue;

            mId = nla_get_u32(tb2[CTRL_ATTR_MCAST_GRP_ID]);
 break;
 }

 return NL_SKIP;
 }
