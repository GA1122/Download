acl_to_ac(sc_file_t *file, unsigned int op, u8 *ac)
{
	const sc_acl_entry_t *acl;
	unsigned int	npins = 0;

	ac[0] = ac[1] = 0;

	if ((acl = sc_file_get_acl_entry(file, op)) == NULL)
		return;

	assert(acl->method != SC_AC_UNKNOWN);
	switch (acl->method) {
	case SC_AC_NEVER:
		ac[0] = 0xC0;
		return;
	case SC_AC_NONE:
		return;
	}

	while (acl) {
		if (acl->method == SC_AC_CHV) {
			 
			if (++npins >= 2)
				continue;
			ac[1] >>= 4;
			ac[1] |= acl->key_ref << 4;
			ac[0] += 0x40;
		}
		if (acl->method == SC_AC_PRO) {
			ac[0] |= acl->key_ref & 0x1f;
		}
		acl = acl->next;
	}
}