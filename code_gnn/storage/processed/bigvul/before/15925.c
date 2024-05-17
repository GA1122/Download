std::string ASF_LegacyManager::NormalizeStringDisplayASCII ( std::string& operand )
{
	std::basic_string<char>::iterator current = operand.begin();
	std::basic_string<char>::iterator end = operand.end();;

	for ( ; (current != end); ++current ) {
		char element = *current;
		if ( ( (element < 0x21) && (element != 0x00)) || (element > 0x7e) ) {
			*current = '?';
		}
	}

	return operand;

}
