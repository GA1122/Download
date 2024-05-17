ZEND_METHOD(exception, __clone)
{
	 
	zend_throw_exception(NULL, "Cannot clone object using __clone()", 0 TSRMLS_CC);
}
