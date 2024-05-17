 base::Lock& GetLockForBlockingDefaultFileTaskRunner() {
   static base::NoDestructor<base::Lock> lock;
   return *lock;
}
