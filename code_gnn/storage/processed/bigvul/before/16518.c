char* param_or_except(const char *attr)
{
	char *tmp = NULL;

	tmp = param(attr);
	if (tmp == NULL || strlen(tmp) <= 0) {
		EXCEPT("Please define config file entry to non-null value: %s", attr);
	}

	return tmp;
}
