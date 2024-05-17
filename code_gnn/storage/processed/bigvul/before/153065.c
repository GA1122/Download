void* MapFont(struct _FPDF_SYSFONTINFO*, int weight, int italic,
              int charset, int pitch_family, const char* face, int* exact) {
  if (!pp::Module::Get())
    return nullptr;

  pp::BrowserFontDescription description;

  if (strcmp(face, "Symbol") == 0)
    return nullptr;

  if (pitch_family & FXFONT_FF_FIXEDPITCH) {
    description.set_family(PP_BROWSERFONT_TRUSTED_FAMILY_MONOSPACE);
  } else if (pitch_family & FXFONT_FF_ROMAN) {
    description.set_family(PP_BROWSERFONT_TRUSTED_FAMILY_SERIF);
  }

  static const struct {
    const char* pdf_name;
    const char* face;
    bool bold;
    bool italic;
  } kPdfFontSubstitutions[] = {
    {"Courier", "Courier New", false, false},
    {"Courier-Bold", "Courier New", true, false},
    {"Courier-BoldOblique", "Courier New", true, true},
    {"Courier-Oblique", "Courier New", false, true},
    {"Helvetica", "Arial", false, false},
    {"Helvetica-Bold", "Arial", true, false},
    {"Helvetica-BoldOblique", "Arial", true, true},
    {"Helvetica-Oblique", "Arial", false, true},
    {"Times-Roman", "Times New Roman", false, false},
    {"Times-Bold", "Times New Roman", true, false},
    {"Times-BoldItalic", "Times New Roman", true, true},
    {"Times-Italic", "Times New Roman", false, true},

    {"MS-PGothic", "MS PGothic", false, false},
    {"MS-Gothic", "MS Gothic", false, false},
    {"MS-PMincho", "MS PMincho", false, false},
    {"MS-Mincho", "MS Mincho", false, false},
    {"\x82\x6C\x82\x72\x82\x6F\x83\x53\x83\x56\x83\x62\x83\x4E",
     "MS PGothic", false, false},
    {"\x82\x6C\x82\x72\x83\x53\x83\x56\x83\x62\x83\x4E",
     "MS Gothic", false, false},
    {"\x82\x6C\x82\x72\x82\x6F\x96\xBE\x92\xA9",
     "MS PMincho", false, false},
    {"\x82\x6C\x82\x72\x96\xBE\x92\xA9",
     "MS Mincho", false, false},
  };

  if (charset == FXFONT_ANSI_CHARSET && (pitch_family & FXFONT_FF_FIXEDPITCH))
    face = "Courier New";

  size_t i;
  for (i = 0; i < arraysize(kPdfFontSubstitutions); ++i) {
    if (strcmp(face, kPdfFontSubstitutions[i].pdf_name) == 0) {
      description.set_face(kPdfFontSubstitutions[i].face);
      if (kPdfFontSubstitutions[i].bold)
        description.set_weight(PP_BROWSERFONT_TRUSTED_WEIGHT_BOLD);
      if (kPdfFontSubstitutions[i].italic)
        description.set_italic(true);
      break;
    }
  }

  if (i == arraysize(kPdfFontSubstitutions)) {
    std::string face_utf8;
    if (base::IsStringUTF8(face)) {
      face_utf8 = face;
    } else {
      std::string encoding;
      if (base::DetectEncoding(face, &encoding)) {
        base::ConvertToUtf8AndNormalize(face, encoding, &face_utf8);
      }
    }

    if (face_utf8.empty())
      return nullptr;

    description.set_face(face_utf8);
    description.set_weight(WeightToBrowserFontTrustedWeight(weight));
    description.set_italic(italic > 0);
  }

  if (!pp::PDF::IsAvailable()) {
    NOTREACHED();
    return nullptr;
  }

  PP_Resource font_resource = pp::PDF::GetFontFileWithFallback(
      pp::InstanceHandle(g_last_instance_id),
      &description.pp_font_description(),
      static_cast<PP_PrivateFontCharset>(charset));
  long res_id = font_resource;
  return reinterpret_cast<void*>(res_id);
}
