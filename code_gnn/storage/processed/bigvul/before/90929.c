int SetThreadName(const std::thread::native_handle_type &thread, const char *name)
{
#if defined(__linux__) || defined(__linux) || defined(linux)
	char name_trunc[16];
	strncpy(name_trunc, name, sizeof(name_trunc));
	name_trunc[sizeof(name_trunc)-1] = '\0';
	return pthread_setname_np(thread, name_trunc);
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
	return 0;
#elif defined(__NetBSD__)
	char name_trunc[PTHREAD_MAX_NAMELEN_NP];
	strncpy(name_trunc, name, sizeof(name_trunc));
	name_trunc[sizeof(name_trunc)-1] = '\0';
	return pthread_setname_np(thread, "%s", (void *)name_trunc);
#elif defined(__OpenBSD__) || defined(__DragonFly__)
	char name_trunc[PTHREAD_MAX_NAMELEN_NP];
	strncpy(name_trunc, name, sizeof(name_trunc));
	name_trunc[sizeof(name_trunc)-1] = '\0';
	pthread_setname_np(thread, name_trunc);
	return 0;
#elif defined(__FreeBSD__)
	char name_trunc[PTHREAD_MAX_NAMELEN_NP];
	strncpy(name_trunc, name, sizeof(name_trunc));
	name_trunc[sizeof(name_trunc)-1] = '\0';
	pthread_set_name_np(thread, name_trunc);
	return 0;
#endif
}