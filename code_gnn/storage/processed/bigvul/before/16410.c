CStarter::WriteAdFiles()
{

	ClassAd* ad;
	const char* dir = this->GetWorkingDir();
	MyString ad_str, filename;
	FILE* fp;
	bool ret_val = true;

	ad = this->jic->jobClassAd();
	if (ad != NULL)
	{
		filename.sprintf("%s%c%s", dir, DIR_DELIM_CHAR, JOB_AD_FILENAME);
		fp = safe_fopen_wrapper_follow(filename.Value(), "w");
		if (!fp)
		{
			dprintf(D_ALWAYS, "Failed to open \"%s\" for to write job ad: "
						"%s (errno %d)\n", filename.Value(),
						strerror(errno), errno);
			ret_val = false;
		}
		else
		{
			ad->SetPrivateAttributesInvisible(true);
			ad->fPrint(fp);
			ad->SetPrivateAttributesInvisible(false);
			fclose(fp);
		}
	}
	else
	{
		ret_val = false;
	}

	ad = this->jic->machClassAd();
	if (ad != NULL)
	{
		filename.sprintf("%s%c%s", dir, DIR_DELIM_CHAR, MACHINE_AD_FILENAME);
		fp = safe_fopen_wrapper_follow(filename.Value(), "w");
		if (!fp)
		{
			dprintf(D_ALWAYS, "Failed to open \"%s\" for to write machine "
						"ad: %s (errno %d)\n", filename.Value(),
					strerror(errno), errno);
			ret_val = false;
		}
		else
		{
			ad->SetPrivateAttributesInvisible(true);
			ad->fPrint(fp);
			ad->SetPrivateAttributesInvisible(false);
			fclose(fp);
		}
	}

	return ret_val;
}
