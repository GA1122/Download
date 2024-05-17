void Plugin::GetReadyStateProperty(NaClSrpcArg* prop_value) {
  PLUGIN_PRINTF(("GetReadyState (this=%p)\n", reinterpret_cast<void*>(this)));
  prop_value->tag = NACL_SRPC_ARG_TYPE_INT;
  prop_value->u.ival = nacl_ready_state();
}
