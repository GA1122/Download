std::vector<std::string> GetSerialPorts(bool &bUseDirectPath)
{
	bUseDirectPath=false;

	std::vector<std::string> ret;
#if defined WIN32

	std::vector<int> ports;
	std::vector<std::string> friendlyNames;
	char szPortName[40];

	EnumSerialFromWMI(ports, friendlyNames);

	bool bFoundPort = false;
	if (!ports.empty())
	{
		bFoundPort = true;
		for (const auto & itt : ports)
		{
			sprintf(szPortName, "COM%d", itt);
			ret.push_back(szPortName);
		}
	}

	if (bFoundPort)
		return ret;

	COMMCONFIG cc;
	DWORD dwSize = sizeof(COMMCONFIG);
	for (int ii = 0; ii < 256; ii++)
	{
		sprintf(szPortName, "COM%d", ii);
		if (GetDefaultCommConfig(szPortName, &cc, &dwSize))
		{
			bFoundPort = true;
			sprintf(szPortName, "COM%d", ii);

			bool bFound = false;
			for (const auto & itt : ret)
			{
				if (itt == szPortName)
				{
					bFound = true;
					break;
				}
			}
			if (!bFound)
				ret.push_back(szPortName);  
		}
	}
	if (!bFoundPort) {
		for (int ii = 0; ii < 256; ii++)
		{
			sprintf(szPortName, "\\\\.\\COM%d", ii);
			bool bSuccess = false;
			HANDLE hPort = ::CreateFile(szPortName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
			if (hPort == INVALID_HANDLE_VALUE) {
				DWORD dwError = GetLastError();
				if (dwError == ERROR_ACCESS_DENIED)
					bSuccess = TRUE;
			}
			else {
				bSuccess = TRUE;
				CloseHandle(hPort);
			}
			if (bSuccess) {
				bFoundPort = true;
				sprintf(szPortName, "COM%d", ii);
				ret.push_back(szPortName);  
			}
		}
	}
	if (!bFoundPort) {
		std::vector<SerialPortInfo> serialports;
		EnumSerialPortsWindows(serialports);
		if (!serialports.empty())
		{
			for (const auto & itt : serialports)
			{
				ret.push_back(itt.szPortName);  
			}
		}
	}

#else

	bool bHaveTtyAMAfree=false;
	std::string sLine = "";
	std::ifstream infile;

	infile.open("/boot/cmdline.txt");
	if (infile.is_open())
	{
		if (!infile.eof())
		{
			getline(infile, sLine);
			bHaveTtyAMAfree=(sLine.find("ttyAMA0")==std::string::npos);
		}
	}

	DIR *d=NULL;
	d=opendir("/dev");
	if (d != NULL)
	{
		struct dirent *de=NULL;
		while ((de = readdir(d)))
		{
                        if ((de->d_type == DT_CHR) || (de->d_type == DT_LNK))
                        {
			std::string fname = de->d_name;
			if (fname.find("ttyUSB")!=std::string::npos)
			{
				ret.push_back("/dev/" + fname);
			}
			else if (fname.find("tty.usbserial")!=std::string::npos)
			{
				bUseDirectPath=true;
				ret.push_back("/dev/" + fname);
			}
			else if (fname.find("ttyACM")!=std::string::npos)
			{
				bUseDirectPath=true;
				ret.push_back("/dev/" + fname);
			}
			else if (fname.find("ttySAC") != std::string::npos)
			{
				bUseDirectPath = true;
				ret.push_back("/dev/" + fname);
			}
#if defined (__FreeBSD__) || defined (__OpenBSD__) || defined (__NetBSD__)
			else if (fname.find("ttyU")!=std::string::npos)
			{
				bUseDirectPath=true;
				ret.push_back("/dev/" + fname);
			}
			else if (fname.find("cuaU")!=std::string::npos)
			{
				bUseDirectPath=true;
				ret.push_back("/dev/" + fname);
			}
#endif
#ifdef __APPLE__
			else if (fname.find("cu.")!=std::string::npos)
			{
				bUseDirectPath=true;
				ret.push_back("/dev/" + fname);
			}
#endif
			if (bHaveTtyAMAfree)
			{
				if (fname.find("ttyAMA0")!=std::string::npos)
				{
					ret.push_back("/dev/" + fname);
					bUseDirectPath=true;
				}
                                        if (fname.find("ttyS0")!=std::string::npos)
                                        {
                                                ret.push_back("/dev/" + fname);
                                                bUseDirectPath=true;
                                        }
                                        if (fname.find("serial")!=std::string::npos)
                                        {
                                                ret.push_back("/dev/" + fname);
                                                bUseDirectPath=true;
                                        }
				}
			}
		}
		closedir(d);
	}
	d=opendir("/dev/usb");
	if (d != NULL)
	{
		struct dirent *de=NULL;
		while ((de = readdir(d)))
		{
			std::string fname = de->d_name;
			if (fname.find("ttyUSB")!=std::string::npos)
			{
				bUseDirectPath=true;
				ret.push_back("/dev/usb/" + fname);
			}
		}
		closedir(d);
	}

#if defined(__linux__) || defined(__linux) || defined(linux)
	d=opendir("/dev/serial/by-id");
	if (d != NULL)
	{
		struct dirent *de=NULL;
		while ((de = readdir(d)))
		{
                        if (de->d_type == DT_LNK)
                        {
				std::string fname = de->d_name;
				ret.push_back("/dev/serial/by-id/" + fname);
			}
		}
		closedir(d);
	}

#endif
#endif
	return ret;
}
