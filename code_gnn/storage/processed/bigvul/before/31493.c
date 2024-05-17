PHP_FUNCTION(radius_auth_open)
{
	radius_descriptor *raddesc;

	raddesc = emalloc(sizeof(radius_descriptor));
	raddesc->radh = rad_auth_open();

	if (raddesc->radh != NULL) {
		ZEND_REGISTER_RESOURCE(return_value, raddesc, le_radius);
		raddesc->id = Z_LVAL_P(return_value);
	} else {
		RETURN_FALSE;
	}
}
