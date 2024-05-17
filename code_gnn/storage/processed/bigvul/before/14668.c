PHPAPI struct lconv *localeconv_r(struct lconv *out)
{
	struct lconv *res;

# ifdef ZTS
	tsrm_mutex_lock( locale_mutex );
# endif

 
#if defined(PHP_WIN32) && _MSC_VER < 1900 && defined(ZTS)
	{
		 
		_locale_t cur = _get_current_locale();

		res = cur->locinfo->lconv;
	}
#else
	 
	res = localeconv();
#endif

	*out = *res;

# ifdef ZTS
	tsrm_mutex_unlock( locale_mutex );
# endif

	return out;
}
