static bool add_value_to_attrib(TALLOC_CTX *mem_ctx, struct ldb_val *value,
				struct ldb_message_element *attrib)
{
	attrib->values = talloc_realloc(mem_ctx,
					attrib->values,
					DATA_BLOB,
					attrib->num_values+1);
	if (attrib->values == NULL)
		return false;

	attrib->values[attrib->num_values].data = talloc_steal(attrib->values,
							       value->data);
	attrib->values[attrib->num_values].length = value->length;
	attrib->num_values += 1;
	return true;
}
