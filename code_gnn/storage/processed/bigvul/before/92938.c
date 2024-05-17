handle_child_line(const char *line, void *data)
{
	UNUSED(data);
	const char *val;
	char buf[1024];

	if (str_startswith(line, "Class:"))
	{
		val = line + sizeof("Class:");
		 
		val += strspn(val, " \t") + sizeof("Class");
		current_device.klass = strtol(val, NULL, 16);
	}
	else if (str_startswith(line, "Vendor:"))
	{
		val = line + sizeof("Vendor:");
		current_device.vendor = strtol(val, NULL, 16);
	}
	else if (str_startswith(line, "Device:"))
	{
		val = line + sizeof("Device:");
		 
		if (!strchr(val, ':'))
			current_device.device = strtol(val, NULL, 16);
	}
	else if (str_startswith(line, "SVendor:"))
	{
		val = line + sizeof("SVendor:");
		current_device.subvendor = strtol(val, NULL, 16);
	}
	else if (str_startswith(line, "SDevice:"))
	{
		val = line + sizeof("SDevice:");
		current_device.subdevice = strtol(val, NULL, 16);
	}
	else if (str_startswith(line, "Rev:"))
	{
		val = line + sizeof("Rev:");
		current_device.revision = strtol(val, NULL, 16);
	}
	else if (str_startswith(line, "ProgIf:"))
	{
		val = line + sizeof("ProgIf:");
		current_device.progif = strtol(val, NULL, 16);
	}
	else if (strspn(line, " \t") == strlen(line))
	{
		 
		snprintf(buf, sizeof(buf), "%04x,%04x,%04x,%04x,%04x,%02x,%02x\n",
			 current_device.klass, current_device.vendor,
			 current_device.device, current_device.subvendor,
			 current_device.subdevice, current_device.revision, current_device.progif);
		lspci_send(buf);
		memset(&current_device, 0, sizeof(current_device));
	}
	else
	{
		logger(Core, Warning, "handle_child_line(), Unrecognized lspci line '%s'", line);
	}
	return True;
}