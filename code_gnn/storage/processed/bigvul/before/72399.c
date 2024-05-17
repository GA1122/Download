static PHP_METHOD(PDOStatement, columnCount)
{
	PHP_STMT_GET_OBJ;
	if (zend_parse_parameters_none() == FAILURE) {
		return;
	}
	RETURN_LONG(stmt->column_count);
}
