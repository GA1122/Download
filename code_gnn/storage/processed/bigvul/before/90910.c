std::vector<std::string> ExecuteCommandAndReturn(const std::string &szCommand, int &returncode)
{
	std::vector<std::string> ret;

	try
	{
		FILE *fp;

		 
#ifdef WIN32
		fp = _popen(szCommand.c_str(), "r");
#else
		fp = popen(szCommand.c_str(), "r");
#endif
		if (fp != NULL)
		{
			char path[1035];
			 
			while (fgets(path, sizeof(path) - 1, fp) != NULL)
			{
				ret.push_back(path);
			}
			 
#ifdef WIN32
			returncode = _pclose(fp);
#else
			returncode = pclose(fp);
#endif
		}
	}
	catch (...)
	{

	}
	return ret;
}
