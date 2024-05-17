int RemoveDir(const std::string &dirnames, std::string &errorPath)
{
	std::vector<std::string> splitresults;
	StringSplit(dirnames, "|", splitresults);
	int returncode = 0;
	if (!splitresults.empty())
	{
#ifdef WIN32
		for (size_t i = 0; i < splitresults.size(); i++)
		{
			if (!file_exist(splitresults[i].c_str()))
				continue;
			size_t s_szLen = strlen(splitresults[i].c_str());
			if (s_szLen < MAX_PATH)
			{
				char deletePath[MAX_PATH + 1];
				strcpy_s(deletePath, splitresults[i].c_str());
				deletePath[s_szLen + 1] = '\0';  

				SHFILEOPSTRUCT shfo = { NULL, FO_DELETE, deletePath, NULL, FOF_SILENT | FOF_NOERRORUI | FOF_NOCONFIRMATION, FALSE, NULL, NULL };
				if (returncode = SHFileOperation(&shfo))
				{
					errorPath = splitresults[i];
					break;
				}
			}
		}
#else
		for (size_t i = 0; i < splitresults.size(); i++)
		{
			if (!file_exist(splitresults[i].c_str()))
				continue;
			ExecuteCommandAndReturn("rm -rf \"" + splitresults[i] + "\"", returncode);
			if (returncode)
			{
				errorPath = splitresults[i];
				break;
			}
		}
#endif
	}
	return returncode;
}