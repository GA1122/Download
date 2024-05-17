bool FoobarIndexIsOdd(const std::string& foobarx_com) {
   return (foobarx_com[6] - '0') % 2 == 1;
 }
