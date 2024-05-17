SPL_API int spl_hash_verify_pos_ex(spl_array_object * intern, HashTable * ht TSRMLS_DC)  
{
	Bucket *p;

 

 
	p = ht->arBuckets[intern->pos_h & ht->nTableMask];
	while (p != NULL) {
		if (p == intern->pos) {
			return SUCCESS;
		}
		p = p->pNext;
	}
 
	spl_array_rewind(intern TSRMLS_CC);
	return FAILURE;

}  
