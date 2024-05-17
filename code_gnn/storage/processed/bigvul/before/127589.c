bool GetOutputDeviceData(XID output,
                         uint16* manufacturer_id,
                         uint32* serial_number,
                         std::string* human_readable_name) {
  if (!IsRandRAvailable())
    return false;

  static Atom edid_property = GetAtom(RR_PROPERTY_RANDR_EDID);

  Display* display = GetXDisplay();

  bool has_edid_property = false;
  int num_properties = 0;
  Atom* properties = XRRListOutputProperties(display, output, &num_properties);
  for (int i = 0; i < num_properties; ++i) {
    if (properties[i] == edid_property) {
      has_edid_property = true;
      break;
    }
  }
  XFree(properties);
  if (!has_edid_property)
    return false;

  Atom actual_type;
  int actual_format;
  unsigned long nitems;
  unsigned long bytes_after;
  unsigned char *prop;
  XRRGetOutputProperty(display,
                       output,
                       edid_property,
                       0,                 
                       128,               
                       false,             
                       false,             
                       AnyPropertyType,   
                       &actual_type,
                       &actual_format,
                       &nitems,
                       &bytes_after,
                       &prop);
  DCHECK_EQ(XA_INTEGER, actual_type);
  DCHECK_EQ(8, actual_format);

  const unsigned int kManufacturerOffset = 8;
  const unsigned int kManufacturerLength = 2;
  const unsigned int kSerialNumberOffset = 12;
  const unsigned int kSerialNumberLength = 4;
  const unsigned int kDescriptorOffset = 54;
  const unsigned int kNumDescriptors = 4;
  const unsigned int kDescriptorLength = 18;
  const unsigned char kMonitorNameDescriptor = 0xfc;
  const unsigned char kUnspecifiedTextDescriptor = 0xfe;

  if (manufacturer_id) {
    if (nitems < kManufacturerOffset + kManufacturerLength) {
      XFree(prop);
      return false;
    }
    *manufacturer_id = *reinterpret_cast<uint16*>(prop + kManufacturerOffset);
#if defined(ARCH_CPU_LITTLE_ENDIAN)
    *manufacturer_id = base::ByteSwap(*manufacturer_id);
#endif
  }

  if (serial_number) {
    if (nitems < kSerialNumberOffset + kSerialNumberLength) {
      XFree(prop);
      return false;
    }
    *serial_number = base::ByteSwapToLE32(
        *reinterpret_cast<uint32*>(prop + kSerialNumberOffset));
  }

  if (!human_readable_name) {
    XFree(prop);
    return true;
  }

  std::string name_candidate;
  human_readable_name->clear();
  for (unsigned int i = 0; i < kNumDescriptors; ++i) {
    if (nitems < kDescriptorOffset + (i + 1) * kDescriptorLength) {
      break;
    }

    unsigned char* desc_buf = prop + kDescriptorOffset + i * kDescriptorLength;
    if (desc_buf[0] == 0 && desc_buf[1] == 0 && desc_buf[2] == 0 &&
        desc_buf[4] == 0) {
      if (desc_buf[3] == kMonitorNameDescriptor) {
        std::string found_name(
            reinterpret_cast<char*>(desc_buf + 5), kDescriptorLength - 5);
        TrimWhitespaceASCII(found_name, TRIM_TRAILING, human_readable_name);
        break;
      } else if (desc_buf[3] == kUnspecifiedTextDescriptor &&
                 name_candidate.empty()) {
        std::string found_name(
            reinterpret_cast<char*>(desc_buf + 5), kDescriptorLength - 5);
        TrimWhitespaceASCII(found_name, TRIM_TRAILING, &name_candidate);
      }
    }
  }
  if (human_readable_name->empty() && !name_candidate.empty())
    *human_readable_name = name_candidate;

  XFree(prop);

  if (human_readable_name->empty())
    return false;

  for (size_t i = 0; i < human_readable_name->size(); ++i) {
    char c = (*human_readable_name)[i];
    if (!isascii(c) || !isprint(c)) {
      human_readable_name->clear();
      return false;
    }
  }

  return true;
}
