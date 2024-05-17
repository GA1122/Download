int sysapi_opsys_version()
{
    if ( ! opsys_version) {
         sysapi_opsys_versioned();  
    }
    return opsys_version;
}
