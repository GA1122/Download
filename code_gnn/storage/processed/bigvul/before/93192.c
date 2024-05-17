parse_interface_number(const char *device)
{
	long devnum;
	char *end;

	devnum = strtol(device, &end, 10);
	if (device != end && *end == '\0') {
		 
		if (devnum <= 0) {
			 
			error("Invalid adapter index");
		}
		return (devnum);
	} else {
		 
		return (-1);
	}
}
