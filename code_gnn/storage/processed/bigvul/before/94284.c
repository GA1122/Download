static enum test_return test_safe_strtoll(void) {
    int64_t val;
    assert(safe_strtoll("123", &val));
    assert(val == 123);
    assert(safe_strtoll("+123", &val));
    assert(val == 123);
    assert(safe_strtoll("-123", &val));
    assert(val == -123);
    assert(!safe_strtoll("", &val));   
    assert(!safe_strtoll("123BOGUS", &val));   
    assert(!safe_strtoll("92837498237498237498029383", &val));  

    assert(!safe_strtoll("18446744073709551615", &val));  
    assert(safe_strtoll("9223372036854775807", &val));  
    assert(val == 9223372036854775807LL);
     
    assert(!safe_strtoll("-9223372036854775809", &val));  

    assert(safe_strtoll(" 123 foo", &val));
    assert(val == 123);
    return TEST_PASS;
}
