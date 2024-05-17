compile_call(CallNode* node, regex_t* reg, ScanEnv* env)
{
  int r;
  int offset;

  r = add_op(reg, OP_CALL);
  if (r != 0) return r;

  COP(reg)->call.addr = 0;  

  offset = COP_CURR_OFFSET_BYTES(reg, call.addr);
  r = unset_addr_list_add(env->unset_addr_list, offset, NODE_CALL_BODY(node));
  return r;
}