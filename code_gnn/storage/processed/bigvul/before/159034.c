std::string WideStringToString(FPDF_WIDESTRING wide_string) {
  return base::UTF16ToUTF8(reinterpret_cast<const base::char16*>(wide_string));
}
