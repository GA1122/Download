static PHP_METHOD(PDOStatement, errorCode)
{
	PHP_STMT_GET_OBJ;

	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}

	if (stmt->error_code[0] == '\0') {
		RETURN_NULL();
	}

	RETURN_STRING(stmt->error_code, 1);
}
