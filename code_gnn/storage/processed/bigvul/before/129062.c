void CopyDebuggee(Debuggee* dst, const Debuggee& src) {
  if (src.tab_id)
    dst->tab_id.reset(new int(*src.tab_id));
  if (src.extension_id)
    dst->extension_id.reset(new std::string(*src.extension_id));
  if (src.target_id)
    dst->target_id.reset(new std::string(*src.target_id));
}
