static bool should_attach_gdb(const debugger_request_t& request) {
 if (request.action == DEBUGGER_ACTION_CRASH) {
 return property_get_bool("debug.debuggerd.wait_for_gdb", false);
 }
 return false;
}
