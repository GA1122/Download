static ssize_t show_tabletExecute(struct device *dev, struct device_attribute *attr, char *buf)
{
	 
	return snprintf(buf, PAGE_SIZE,
			"Write anything to this file to program your tablet.\n");
}
