sys_dyld_init()
{
	int retCode = 0;
	int err = 0;
	if (!_dyld_present()) {
		retCode=1;
	}
	else {
      err = _dyld_func_lookup("__dyld_NSAddImage",(unsigned long*)&ltdl_NSAddImage);
      err = _dyld_func_lookup("__dyld_NSLookupSymbolInImage",(unsigned long*)&ltdl_NSLookupSymbolInImage);
      err = _dyld_func_lookup("__dyld_NSIsSymbolNameDefinedInImage",(unsigned long*)&ltdl_NSIsSymbolNameDefinedInImage);
      err = _dyld_func_lookup("__dyld_NSMakePrivateModulePublic",(unsigned long*)&ltdl_NSMakePrivateModulePublic);
    }
 return retCode;
}
