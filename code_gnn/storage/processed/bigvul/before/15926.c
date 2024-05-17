std::string ASF_LegacyManager::NormalizeStringTrailingNull ( std::string& operand )
{
	if ( ( operand.size() > 0) && (operand[operand.size() - 1] != '\0') ) {
		operand.append ( 1, '\0' );
	}

	return operand;

}
