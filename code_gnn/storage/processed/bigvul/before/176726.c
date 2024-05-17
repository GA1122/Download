static dev_t ashmem_rdev()
{
 static dev_t __ashmem_rdev;
 static pthread_mutex_t __ashmem_rdev_lock = PTHREAD_MUTEX_INITIALIZER;

    pthread_mutex_lock(&__ashmem_rdev_lock);

 dev_t rdev = __ashmem_rdev;
 if (!rdev) {
 int fd = TEMP_FAILURE_RETRY(open("/dev/ashmem", O_RDONLY));
 if (fd >= 0) {
 struct stat st;

 int ret = TEMP_FAILURE_RETRY(fstat(fd, &st));
            close(fd);
 if ((ret >= 0) && S_ISCHR(st.st_mode)) {
                rdev = __ashmem_rdev = st.st_rdev;
 }
 }
 }

    pthread_mutex_unlock(&__ashmem_rdev_lock);

 return rdev;
}
