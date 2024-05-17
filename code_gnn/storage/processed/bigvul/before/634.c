_PUBLIC_ struct ldap_message *new_ldap_message(TALLOC_CTX *mem_ctx)
{
	return talloc_zero(mem_ctx, struct ldap_message);
}
