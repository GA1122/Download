static void config_permissions(void)
{
 struct __user_cap_header_struct header;
 struct __user_cap_data_struct cap;

    bdt_log("set_aid_and_cap : pid %d, uid %d gid %d", getpid(), getuid(), getgid());

    header.pid = 0;

    prctl(PR_SET_KEEPCAPS, 1, 0, 0, 0);

    setuid(AID_BLUETOOTH);
    setgid(AID_BLUETOOTH);

    header.version = _LINUX_CAPABILITY_VERSION;

    cap.effective = cap.permitted =  cap.inheritable =
 1 << CAP_NET_RAW |
 1 << CAP_NET_ADMIN |
 1 << CAP_NET_BIND_SERVICE |
 1 << CAP_SYS_RAWIO |
 1 << CAP_SYS_NICE |
 1 << CAP_SETGID;

    capset(&header, &cap);
    setgroups(sizeof(groups)/sizeof(groups[0]), groups);
}
