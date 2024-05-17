static PHP_GINIT_FUNCTION(bcmath)
{
	bcmath_globals->bc_precision = 0;
	bc_init_numbers(TSRMLS_C);
}
