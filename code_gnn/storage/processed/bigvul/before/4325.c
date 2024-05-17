static void php_putenv_destructor(putenv_entry *pe)  
{
	if (pe->previous_value) {
#if _MSC_VER >= 1300
		 
		SetEnvironmentVariable(pe->key, "bugbug");
#endif
		putenv(pe->previous_value);
# if defined(PHP_WIN32)
		efree(pe->previous_value);
# endif
	} else {
# if HAVE_UNSETENV
		unsetenv(pe->key);
# elif defined(PHP_WIN32)
		SetEnvironmentVariable(pe->key, NULL);
# else
		char **env;

		for (env = environ; env != NULL && *env != NULL; env++) {
			if (!strncmp(*env, pe->key, pe->key_len) && (*env)[pe->key_len] == '=') {	 
				*env = "";
				break;
			}
		}
# endif
	}
#ifdef HAVE_TZSET
	 
	if (!strncmp(pe->key, "TZ", pe->key_len)) {
		tzset();
	}
#endif

	efree(pe->putenv_string);
	efree(pe->key);
}
 
