static inline int ebt_obj_to_user(char __user *um, const char *_name,
				  const char *data, int entrysize,
				  int usersize, int datasize)
{
	char name[EBT_FUNCTION_MAXNAMELEN] = {0};

	 
	strlcpy(name, _name, sizeof(name));
	if (copy_to_user(um, name, EBT_FUNCTION_MAXNAMELEN) ||
	    put_user(datasize, (int __user *)(um + EBT_FUNCTION_MAXNAMELEN)) ||
	    xt_data_to_user(um + entrysize, data, usersize, datasize,
			    XT_ALIGN(datasize)))
		return -EFAULT;

	return 0;
}
