wifi_error wifi_initialize(wifi_handle *handle)
{
    srand(getpid());

    ALOGI("Initializing wifi");
    hal_info *info = (hal_info *)malloc(sizeof(hal_info));
 if (info == NULL) {
        ALOGE("Could not allocate hal_info");
 return WIFI_ERROR_UNKNOWN;
 }

    memset(info, 0, sizeof(*info));

    ALOGI("Creating socket");
 if (socketpair(AF_UNIX, SOCK_STREAM, 0, info->cleanup_socks) == -1) {
        ALOGE("Could not create cleanup sockets");
        free(info);
 return WIFI_ERROR_UNKNOWN;
 }

 struct nl_sock *cmd_sock = wifi_create_nl_socket(WIFI_HAL_CMD_SOCK_PORT);
 if (cmd_sock == NULL) {
        ALOGE("Could not create handle");
        free(info);
 return WIFI_ERROR_UNKNOWN;
 }

 struct nl_sock *event_sock = wifi_create_nl_socket(WIFI_HAL_EVENT_SOCK_PORT);
 if (event_sock == NULL) {
        ALOGE("Could not create handle");
        nl_socket_free(cmd_sock);
        free(info);
 return WIFI_ERROR_UNKNOWN;
 }

 struct nl_cb *cb = nl_socket_get_cb(event_sock);
 if (cb == NULL) {
        ALOGE("Could not create handle");
        nl_socket_free(cmd_sock);
        nl_socket_free(event_sock);
        free(info);
 return WIFI_ERROR_UNKNOWN;
 }

    nl_cb_set(cb, NL_CB_SEQ_CHECK, NL_CB_CUSTOM, internal_no_seq_check, info);
    nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM, internal_valid_message_handler, info);
    nl_cb_put(cb);

    info->cmd_sock = cmd_sock;
    info->event_sock = event_sock;
    info->clean_up = false;
    info->in_event_loop = false;

    info->event_cb = (cb_info *)malloc(sizeof(cb_info) * DEFAULT_EVENT_CB_SIZE);
    info->alloc_event_cb = DEFAULT_EVENT_CB_SIZE;
    info->num_event_cb = 0;

    info->cmd = (cmd_info *)malloc(sizeof(cmd_info) * DEFAULT_CMD_SIZE);
    info->alloc_cmd = DEFAULT_CMD_SIZE;
    info->num_cmd = 0;

    info->nl80211_family_id = genl_ctrl_resolve(cmd_sock, "nl80211");
 if (info->nl80211_family_id < 0) {
        ALOGE("Could not resolve nl80211 familty id");
        nl_socket_free(cmd_sock);
        nl_socket_free(event_sock);
        free(info);
 return WIFI_ERROR_UNKNOWN;
 }

    pthread_mutex_init(&info->cb_lock, NULL);

 *handle = (wifi_handle) info;

    wifi_add_membership(*handle, "scan");
    wifi_add_membership(*handle, "mlme");
    wifi_add_membership(*handle, "regulatory");
    wifi_add_membership(*handle, "vendor");

    wifi_init_interfaces(*handle);


    ALOGI("Initialized Wifi HAL Successfully; vendor cmd = %d", NL80211_CMD_VENDOR);
 return WIFI_SUCCESS;
}
