void Plugin::GetLastError(NaClSrpcArg* prop_value) {
  PLUGIN_PRINTF(("GetLastError (this=%p)\n", reinterpret_cast<void*>(this)));
  prop_value->tag = NACL_SRPC_ARG_TYPE_STRING;
  prop_value->arrays.str = strdup(last_error_string().c_str());
}
