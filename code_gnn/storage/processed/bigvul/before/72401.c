static PHP_METHOD(PDOStatement, setFetchMode)
{
	PHP_STMT_GET_OBJ;

	RETVAL_BOOL(
		pdo_stmt_setup_fetch_mode(INTERNAL_FUNCTION_PARAM_PASSTHRU,
			stmt, 0) == SUCCESS ? 1 : 0
		);
}
