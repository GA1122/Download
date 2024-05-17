NaClDesc* MakeNaClDescCustom(NaClIPCAdapter* adapter) {
  NaClDescCustomFuncs funcs = NACL_DESC_CUSTOM_FUNCS_INITIALIZER;
  funcs.Destroy = NaClDescCustomDestroy;
  funcs.SendMsg = NaClDescCustomSendMsg;
  funcs.RecvMsg = NaClDescCustomRecvMsg;
  return NaClDescMakeCustomDesc(new DescThunker(adapter), &funcs);
}
