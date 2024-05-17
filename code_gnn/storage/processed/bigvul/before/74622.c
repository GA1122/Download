static char *skip_over_word(char *cp)
{
	char ch;

	while ((ch = *cp)) {
		 
		if (ch == '\\') {
			cp++;
			if (*cp == '\0')
				break;
			cp++;
			continue;
		}
		if (isspace(ch) || ch == '<' || ch == '>')
			break;
		cp++;
	}
	return cp;
}