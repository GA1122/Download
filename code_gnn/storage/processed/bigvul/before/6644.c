AviaryScheddPlugin::processDirtyJobs()
{
	BeginTransaction();

	while (!dirtyJobs->empty()) {
		DirtyJobEntry entry = dirtyJobs->front(); dirtyJobs->pop_front();
		string key = entry.first;
		string name = entry.second.first;
		int value = entry.second.second;

		processJob(key.c_str(), name.c_str(), value);
	}

	CommitTransaction();

	isHandlerRegistered = false;
}
