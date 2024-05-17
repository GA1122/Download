iperf_exchange_results(struct iperf_test *test)
{
    if (test->role == 'c') {
         
	if (send_results(test) < 0)
            return -1;
         
        if (get_results(test) < 0)
            return -1;
    } else {
         
        if (get_results(test) < 0)
            return -1;
         
	if (send_results(test) < 0)
            return -1;
    }
    return 0;
}
