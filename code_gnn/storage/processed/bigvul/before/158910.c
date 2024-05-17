void DeleteFont(FPDF_SYSFONTINFO*, void* font_id) {
  long res_id = reinterpret_cast<long>(font_id);
  pp::Module::Get()->core()->ReleaseResource(res_id);
}
