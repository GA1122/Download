void LoadPaper(const wchar_t* printer,
               const wchar_t* port,
               const DEVMODE* devmode,
               PrinterSemanticCapsAndDefaults* caps) {
  static const size_t kToUm = 100;   
  static const size_t kMaxPaperName = 64;

  struct PaperName {
    wchar_t chars[kMaxPaperName];
  };

  DCHECK_EQ(sizeof(PaperName), sizeof(wchar_t) * kMaxPaperName);

  std::vector<PaperName> names;
  GetDeviceCapabilityArray(printer, port, DC_PAPERNAMES, &names);

  std::vector<POINT> sizes;
  GetDeviceCapabilityArray(printer, port, DC_PAPERSIZE, &sizes);

  std::vector<WORD> ids;
  GetDeviceCapabilityArray(printer, port, DC_PAPERS, &ids);

  DCHECK_EQ(ids.size(), sizes.size());
  DCHECK_EQ(names.size(), sizes.size());

  if (ids.size() != sizes.size())
    ids.clear();
  if (names.size() != sizes.size())
    names.clear();

  for (size_t i = 0; i < sizes.size(); ++i) {
    PrinterSemanticCapsAndDefaults::Paper paper;
    paper.size_um.SetSize(sizes[i].x * kToUm, sizes[i].y * kToUm);
    if (!names.empty()) {
      const wchar_t* name_start = names[i].chars;
      base::string16 tmp_name(name_start, kMaxPaperName);
      tmp_name = tmp_name.c_str();
      paper.display_name = base::WideToUTF8(tmp_name);
    }
    if (!ids.empty())
      paper.vendor_id = base::UintToString(ids[i]);
    caps->papers.push_back(paper);
  }

  if (devmode) {
    if (devmode->dmFields & DM_PAPERSIZE) {
      for (size_t i = 0; i < ids.size(); ++i) {
        if (ids[i] == devmode->dmPaperSize) {
          DCHECK_EQ(ids.size(), caps->papers.size());
          caps->default_paper = caps->papers[i];
          break;
        }
      }
    }

    gfx::Size default_size;
    if (devmode->dmFields & DM_PAPERWIDTH)
      default_size.set_width(devmode->dmPaperWidth * kToUm);
    if (devmode->dmFields & DM_PAPERLENGTH)
      default_size.set_height(devmode->dmPaperLength * kToUm);

    if (!default_size.IsEmpty()) {
      if (default_size != caps->default_paper.size_um)
        caps->default_paper = PrinterSemanticCapsAndDefaults::Paper();
      caps->default_paper.size_um = default_size;
    }
  }
}
