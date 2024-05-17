void Plugin::GetExitStatus(NaClSrpcArg* prop_value) {
  PLUGIN_PRINTF(("GetExitStatus (this=%p)\n", reinterpret_cast<void*>(this)));
  prop_value->tag = NACL_SRPC_ARG_TYPE_INT;
  prop_value->u.ival = exit_status();
}
