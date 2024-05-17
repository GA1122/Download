	__releases(nl_table_lock)
{
	read_unlock(&nl_table_lock);
}
