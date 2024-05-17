std::string &stdstring_ltrim(std::string &s)
{
	while (!s.empty())
	{
		if (s[0] != ' ')
			return s;
		s = s.substr(1);
	}
	return s;
}
