AviaryScheddPlugin::initialize()
{
	static bool skip = false;
	if (skip) return; skip = true;

	ClassAd *ad = GetNextJob(1);
	while (ad != NULL) {
		MyString key;
		PROC_ID id;
		int value;

		if (!ad->LookupInteger(ATTR_CLUSTER_ID, id.cluster)) {
			EXCEPT("%s on job is missing or not an integer", ATTR_CLUSTER_ID);
		}
		if (!ad->LookupInteger(ATTR_PROC_ID, id.proc)) {
			EXCEPT("%s on job is missing or not an integer", ATTR_PROC_ID);
		}
		if (!ad->LookupInteger(ATTR_JOB_STATUS, value)) {
			EXCEPT("%s on job is missing or not an integer", ATTR_JOB_STATUS);
		}

		key.sprintf("%d.%d", id.cluster, id.proc);

		processJob(key.Value(), ATTR_JOB_STATUS, value);

		FreeJobAd(ad);
		ad = GetNextJob(0);
	}
 
	m_initialized = true;
}
