sysapi_opsys(void)
{
    if (_sysapi_opsys_is_versioned) {
        return sysapi_opsys_versioned();
    }
    return "WINDOWS";
}
