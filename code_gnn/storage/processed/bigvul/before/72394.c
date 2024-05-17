static PHP_METHOD(PDOStatement, rowCount)
{
	PHP_STMT_GET_OBJ;

	RETURN_LONG(stmt->row_count);
}
