static enum test_return test_safe_strtoul(void) {
    uint32_t val;
    assert(safe_strtoul("123", &val));
    assert(val == 123);
    assert(safe_strtoul("+123", &val));
    assert(val == 123);
    assert(!safe_strtoul("", &val));   
    assert(!safe_strtoul("123BOGUS", &val));   
     

    assert(safe_strtoul("4294967295", &val));  
    assert(val == 4294967295L);
     
    assert(!safe_strtoul("-1", &val));   
    return TEST_PASS;
}
