store_tabletExecute(struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
	struct aiptek *aiptek = dev_get_drvdata(dev);

	 
	memcpy(&aiptek->curSetting, &aiptek->newSetting,
	       sizeof(struct aiptek_settings));

	if (aiptek_program_tablet(aiptek) < 0)
		return -EIO;

	return count;
}
