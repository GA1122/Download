static void wait_for_user_action(const debugger_request_t& request) {
  ALOGI("***********************************************************\n"
 "* Process %d has been suspended while crashing.\n"
 "* To attach gdbserver and start gdb, run this on the host:\n"
 "*\n"
 "*     gdbclient.py -p %d\n"
 "*\n"
 "* Wait for gdb to start, then press the VOLUME DOWN key\n"
 "* to let the process continue crashing.\n"
 "***********************************************************",
        request.pid, request.tid);

 while (true) {
    input_event e;
 if (get_event(&e, -1) == 0) {
 if (e.type == EV_KEY && e.code == KEY_VOLUMEDOWN && e.value == 0) {
 break;
 }
 }
 }

  ALOGI("debuggerd resuming process %d", request.pid);
}
