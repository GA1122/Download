bool asn1_check_OID(struct asn1_data *data, const char *OID)
{
	char *id;

	if (!asn1_read_OID(data, data, &id)) return false;

	if (strcmp(id, OID) != 0) {
		talloc_free(id);
		data->has_error = true;
		return false;
	}
	talloc_free(id);
	return true;
}
