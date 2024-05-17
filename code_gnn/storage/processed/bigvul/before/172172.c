static void section_free(void *ptr) {
 if (!ptr)
 return;

 section_t *section = ptr;
  osi_free(section->name);
  list_free(section->entries);
  osi_free(section);
}
