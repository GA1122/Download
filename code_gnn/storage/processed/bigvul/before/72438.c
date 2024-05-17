static int pdo_stmt_verify_mode(pdo_stmt_t *stmt, long mode, int fetch_all TSRMLS_DC)  
{
	int flags = mode & PDO_FETCH_FLAGS;

	mode = mode & ~PDO_FETCH_FLAGS;

	if (mode < 0 || mode > PDO_FETCH__MAX) {
		pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "invalid fetch mode" TSRMLS_CC);
		return 0;
	}

	if (mode == PDO_FETCH_USE_DEFAULT) {
		flags = stmt->default_fetch_type & PDO_FETCH_FLAGS;
		mode = stmt->default_fetch_type & ~PDO_FETCH_FLAGS;
	}

	switch(mode) {
	case PDO_FETCH_FUNC:
		if (!fetch_all) {
			pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "PDO::FETCH_FUNC is only allowed in PDOStatement::fetchAll()" TSRMLS_CC);
			return 0;
		}
		return 1;

	case PDO_FETCH_LAZY:
		if (fetch_all) {
			pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "PDO::FETCH_LAZY can't be used with PDOStatement::fetchAll()" TSRMLS_CC);
			return 0;
		}
		 

	default:
		if ((flags & PDO_FETCH_SERIALIZE) == PDO_FETCH_SERIALIZE) {
			pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "PDO::FETCH_SERIALIZE can only be used together with PDO::FETCH_CLASS" TSRMLS_CC);
			return 0;
		}
		if ((flags & PDO_FETCH_CLASSTYPE) == PDO_FETCH_CLASSTYPE) {
			pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "PDO::FETCH_CLASSTYPE can only be used together with PDO::FETCH_CLASS" TSRMLS_CC);
			return 0;
		}
		if (mode >= PDO_FETCH__MAX) {
			pdo_raise_impl_error(stmt->dbh, stmt, "HY000", "invalid fetch mode" TSRMLS_CC);
			return 0;
		}
		 

	case PDO_FETCH_CLASS:
		return 1;
	}
}
 