 virtual bool MyIpAddressEx(std::string* ip_address_list) {
    my_ip_address_ex_count++;
 *ip_address_list = my_ip_address_ex_result;
 return !my_ip_address_ex_result.empty();
 }
