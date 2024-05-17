const char *get_camera_metadata_section_name(uint32_t tag) {
 uint32_t tag_section = tag >> 16;
 if (tag_section >= VENDOR_SECTION && vendor_tag_ops != NULL) {
 return vendor_tag_ops->get_section_name(
            vendor_tag_ops,
            tag);
 }
 if (tag_section >= ANDROID_SECTION_COUNT) {
 return NULL;
 }
 return camera_metadata_section_names[tag_section];
}
