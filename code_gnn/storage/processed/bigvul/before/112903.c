 void SetFreeDiskSpaceGetterForTesting(FreeDiskSpaceGetterInterface* getter) {
   delete global_free_disk_getter_for_testing;   
   global_free_disk_getter_for_testing = getter;
}
