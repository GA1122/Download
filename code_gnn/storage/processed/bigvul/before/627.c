static bool add_attrib_to_array_talloc(TALLOC_CTX *mem_ctx,
				       const struct ldb_message_element *attrib,
				       struct ldb_message_element **attribs,
				       int *num_attribs)
{
	*attribs = talloc_realloc(mem_ctx,
				  *attribs,
				  struct ldb_message_element,
				  *num_attribs+1);

	if (*attribs == NULL)
		return false;

	(*attribs)[*num_attribs] = *attrib;
	talloc_steal(*attribs, attrib->values);
	talloc_steal(*attribs, attrib->name);
	*num_attribs += 1;
	return true;
}
