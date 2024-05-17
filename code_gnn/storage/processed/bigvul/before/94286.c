static enum test_return test_safe_strtoull(void) {
    uint64_t val;
    assert(safe_strtoull("123", &val));
    assert(val == 123);
    assert(safe_strtoull("+123", &val));
    assert(val == 123);
    assert(!safe_strtoull("", &val));   
    assert(!safe_strtoull("123BOGUS", &val));   
    assert(!safe_strtoull("92837498237498237498029383", &val));  

    assert(safe_strtoull("18446744073709551615", &val));  
    assert(val == 18446744073709551615ULL);
    assert(!safe_strtoull("18446744073709551616", &val));  
    assert(!safe_strtoull("-1", &val));   
    return TEST_PASS;
}
