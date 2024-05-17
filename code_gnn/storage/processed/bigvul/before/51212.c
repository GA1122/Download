parse_rockridge_SL1(struct file_info *file, const unsigned char *data,
    int data_length)
{
	const char *separator = "";

	if (!file->symlink_continues || file->symlink.length < 1)
		archive_string_empty(&file->symlink);
	file->symlink_continues = 0;

	 
	if (data_length < 1)
		return;
	switch(*data) {
	case 0:
		break;
	case 1:
		file->symlink_continues = 1;
		break;
	default:
		return;
	}
	++data;   
	--data_length;

	 
	while (data_length >= 2) {
		unsigned char flag = *data++;
		unsigned char nlen = *data++;
		data_length -= 2;

		archive_strcat(&file->symlink, separator);
		separator = "/";

		switch(flag) {
		case 0:  
			if (data_length < nlen)
				return;
			archive_strncat(&file->symlink,
			    (const char *)data, nlen);
			break;
		case 0x01:  
			if (data_length < nlen)
				return;
			archive_strncat(&file->symlink,
			    (const char *)data, nlen);
			separator = "";
			break;
		case 0x02:  
			archive_strcat(&file->symlink, ".");
			break;
		case 0x04:  
			archive_strcat(&file->symlink, "..");
			break;
		case 0x08:  
			archive_strcat(&file->symlink, "/");
			separator = "";
			break;
		case 0x10:  
			archive_string_empty(&file->symlink);
			archive_strcat(&file->symlink, "ROOT");
			break;
		case 0x20:  
			archive_strcat(&file->symlink, "hostname");
			break;
		default:
			 
			return;
		}
		data += nlen;
		data_length -= nlen;
	}
}
