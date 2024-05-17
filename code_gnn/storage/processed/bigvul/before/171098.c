int find_camera_metadata_entry(camera_metadata_t *src,
 uint32_t tag,
 camera_metadata_entry_t *entry) {
 if (src == NULL) return ERROR;

 uint32_t index;
 if (src->flags & FLAG_SORTED) {
 camera_metadata_buffer_entry_t *search_entry = NULL;
 camera_metadata_buffer_entry_t key;
        key.tag = tag;
        search_entry = bsearch(&key,
                get_entries(src),
                src->entry_count,
 sizeof(camera_metadata_buffer_entry_t),
                compare_entry_tags);
 if (search_entry == NULL) return NOT_FOUND;
        index = search_entry - get_entries(src);
 } else {
 camera_metadata_buffer_entry_t *search_entry = get_entries(src);
 for (index = 0; index < src->entry_count; index++, search_entry++) {
 if (search_entry->tag == tag) {
 break;
 }
 }
 if (index == src->entry_count) return NOT_FOUND;
 }

 return get_camera_metadata_entry(src, index,
            entry);
}
