std::string GenerateUUID()  
{
	std::srand((unsigned int)std::time(nullptr));
	std::string uuid = std::string(36, ' ');

	uuid[8] = '-';
	uuid[13] = '-';
	uuid[14] = '4';  
	uuid[18] = '-';
	uuid[23] = '-';

	for (size_t ii = 0; ii < uuid.size(); ii++)
	{
		if (uuid[ii] == ' ')
		{
			uuid[ii] = hexCHARS[(ii == 19) ? (8 + (std::rand() & 0x03)) : std::rand() & 0x0F];
		}
	}
	return uuid;
}
