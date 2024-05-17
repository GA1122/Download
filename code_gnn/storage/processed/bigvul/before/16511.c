param_boolean_int( const char *name, int default_value ) {
    bool default_bool;
    default_bool = default_value == 0 ? false : true;
    return param_boolean(name, default_bool) ? 1 : 0;
}
