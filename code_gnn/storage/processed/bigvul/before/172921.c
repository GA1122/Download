static void sig_handler(int signo) {
 if (signo == SIGINT) {
    fprintf(stderr, "Received SIGINT\n");
    CALL_AND_WAIT(bt_interface->disable(), adapter_state_changed);
    fprintf(stderr, "BT adapter is down\n");
    exit(1);
 }
}
