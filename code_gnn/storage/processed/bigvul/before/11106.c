void destroy_phar_manifest_entry(zval *zv)  
{
	phar_entry_info *entry = Z_PTR_P(zv);
	destroy_phar_manifest_entry_int(entry);
	pefree(entry, entry->is_persistent);
}
 
