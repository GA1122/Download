vrend_insert_format(struct vrend_format_table *entry, uint32_t bindings)
{
   tex_conv_table[entry->format] = *entry;
   tex_conv_table[entry->format].bindings = bindings;
}
