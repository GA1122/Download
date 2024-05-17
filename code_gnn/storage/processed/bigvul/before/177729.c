 virtual bool MyIpAddress(std::string* ip_address) {
    my_ip_address_count++;
 *ip_address = my_ip_address_result;
 return !my_ip_address_result.empty();
 }
