    bool UnSetVarImpl(const char* variable_name) {
  #if defined(OS_POSIX)
    return unsetenv(variable_name) == 0;
//     return !unsetenv(variable_name);
  #elif defined(OS_WIN)
    return ::SetEnvironmentVariable(ASCIIToWide(variable_name).c_str(),
                                    NULL) != 0;
//      
//     return !!SetEnvironmentVariable(UTF8ToWide(variable_name).c_str(), NULL);
  #endif
    }