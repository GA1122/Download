static PHP_METHOD(PDOStatement, bindParam)
{
	PHP_STMT_GET_OBJ;
	RETURN_BOOL(register_bound_param(INTERNAL_FUNCTION_PARAM_PASSTHRU, stmt, TRUE));
}
