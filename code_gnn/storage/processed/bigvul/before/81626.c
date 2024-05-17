b64reverse(char letter)
{
	if ((letter >= 'A') && (letter <= 'Z')) {
		return letter - 'A';
	}
	if ((letter >= 'a') && (letter <= 'z')) {
		return letter - 'a' + 26;
	}
	if ((letter >= '0') && (letter <= '9')) {
		return letter - '0' + 52;
	}
	if (letter == '+') {
		return 62;
	}
	if (letter == '/') {
		return 63;
	}
	if (letter == '=') {
		return 255;  
	}
	return 254;  
}