static int compare_entry_tags(const void *p1, const void *p2) {
 uint32_t tag1 = ((camera_metadata_buffer_entry_t*)p1)->tag;
 uint32_t tag2 = ((camera_metadata_buffer_entry_t*)p2)->tag;
 return  tag1 < tag2 ? -1 :
            tag1 == tag2 ? 0 :
 1;
}
