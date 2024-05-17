int nicklist_compare(NICK_REC *p1, NICK_REC *p2, const char *nick_prefix)
{
	int i;

	if (p1 == NULL) return -1;
	if (p2 == NULL) return 1;

	if (p1->prefixes[0] == p2->prefixes[0])
		return g_ascii_strcasecmp(p1->nick, p2->nick);

	if (!p1->prefixes[0])
		return 1;
	if (!p2->prefixes[0])
		return -1;

	 

	for (i = 0; nick_prefix[i] != '\0'; i++) {
		if (p1->prefixes[0] == nick_prefix[i])
			return -1;
		if (p2->prefixes[0] == nick_prefix[i])
			return 1;
	}

	 
	return g_ascii_strcasecmp(p1->nick, p2->nick);
}
