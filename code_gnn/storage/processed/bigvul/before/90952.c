std::string &stdstring_rtrim(std::string &s)
{
	while (!s.empty())
	{
		if (s[s.size() - 1] != ' ')
			return s;
		s = s.substr(0, s.size() - 1);
	}
	return s;
}
