PHP_HASH_API int php_hash_copy(const void *ops, void *orig_context, void *dest_context)  
{
	php_hash_ops *hash_ops = (php_hash_ops *)ops;

	memcpy(dest_context, orig_context, hash_ops->context_size);
	return SUCCESS;
}
 
