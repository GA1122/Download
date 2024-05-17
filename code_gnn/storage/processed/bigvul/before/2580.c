findJsonbValueFromContainerLen(JsonbContainer *container, uint32 flags,
							   char *key, uint32 keylen)
{
	JsonbValue	k;

	k.type = jbvString;
	k.val.string.val = key;
	k.val.string.len = keylen;

	return findJsonbValueFromContainer(container, flags, &k);
}
