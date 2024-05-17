static void igmp_timer_expire(unsigned long data)
{
	struct ip_mc_list *im=(struct ip_mc_list *)data;
	struct in_device *in_dev = im->interface;

	spin_lock(&im->lock);
	im->tm_running = 0;

	if (im->unsolicit_count) {
		im->unsolicit_count--;
		igmp_start_timer(im, IGMP_Unsolicited_Report_Interval);
	}
	im->reporter = 1;
	spin_unlock(&im->lock);

	if (IGMP_V1_SEEN(in_dev))
		igmp_send_report(in_dev, im, IGMP_HOST_MEMBERSHIP_REPORT);
	else if (IGMP_V2_SEEN(in_dev))
		igmp_send_report(in_dev, im, IGMPV2_HOST_MEMBERSHIP_REPORT);
	else
		igmp_send_report(in_dev, im, IGMPV3_HOST_MEMBERSHIP_REPORT);

	ip_ma_put(im);
}