int get_camera_metadata_tag_type(uint32_t tag) {
 uint32_t tag_section = tag >> 16;
 if (tag_section >= VENDOR_SECTION && vendor_tag_ops != NULL) {
 return vendor_tag_ops->get_tag_type(
            vendor_tag_ops,
            tag);
 }
 if (tag_section >= ANDROID_SECTION_COUNT ||
            tag >= camera_metadata_section_bounds[tag_section][1] ) {
 return -1;
 }
 uint32_t tag_index = tag & 0xFFFF;
 return tag_info[tag_section][tag_index].tag_type;
}
