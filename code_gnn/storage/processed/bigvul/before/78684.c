static unsigned int acl_to_byte_44(const struct sc_acl_entry *e, u8* p_bNumber)
{
	 
	if (e == (sc_acl_entry_t *) 1)            
		return SC_AC_NEVER;
	else if ((e == (sc_acl_entry_t *) 2) ||   
	         (e == (sc_acl_entry_t *) 3) ||   
	         (e == (sc_acl_entry_t *) 0))
		return SC_AC_NONE;

	 
	*p_bNumber = e->key_ref;
	return(e->method);
}
