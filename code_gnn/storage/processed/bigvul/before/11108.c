static void mime_type_dtor(zval *zv)
{
	free(Z_PTR_P(zv));
}
