void TabLoader::Observe(int type,
                        const content::NotificationSource& source,
                        const content::NotificationDetails& details) {
  switch (type) {
    case content::NOTIFICATION_LOAD_START: {
      NavigationController* tab =
          content::Source<NavigationController>(source).ptr();
      RenderWidgetHost* render_widget_host = GetRenderWidgetHost(tab);
      DCHECK(render_widget_host);
      render_widget_hosts_loading_.insert(render_widget_host);
      break;
    }
    case content::NOTIFICATION_WEB_CONTENTS_DESTROYED: {
      WebContents* web_contents = content::Source<WebContents>(source).ptr();
      if (!got_first_paint_) {
        RenderWidgetHost* render_widget_host =
            GetRenderWidgetHost(&web_contents->GetController());
        render_widget_hosts_loading_.erase(render_widget_host);
      }
      HandleTabClosedOrLoaded(&web_contents->GetController());
      break;
    }
    case content::NOTIFICATION_LOAD_STOP: {
      NavigationController* tab =
          content::Source<NavigationController>(source).ptr();
      render_widget_hosts_to_paint_.insert(GetRenderWidgetHost(tab));
      HandleTabClosedOrLoaded(tab);
      break;
    }
    case content::NOTIFICATION_RENDER_WIDGET_HOST_DID_PAINT: {
      if (!got_first_paint_) {
        RenderWidgetHost* render_widget_host =
            content::Source<RenderWidgetHost>(source).ptr();
        if (render_widget_hosts_to_paint_.find(render_widget_host) !=
            render_widget_hosts_to_paint_.end()) {
          got_first_paint_ = true;
          base::TimeDelta time_to_paint =
              base::TimeTicks::Now() - restore_started_;
          UMA_HISTOGRAM_CUSTOM_TIMES(
              "SessionRestore.FirstTabPainted",
              time_to_paint,
              base::TimeDelta::FromMilliseconds(10),
              base::TimeDelta::FromSeconds(100),
              100);
          std::string time_for_count =
              base::StringPrintf("SessionRestore.FirstTabPainted_%d",
                                 tab_count_);
          base::Histogram* counter_for_count =
              base::Histogram::FactoryTimeGet(
                  time_for_count,
                  base::TimeDelta::FromMilliseconds(10),
                  base::TimeDelta::FromSeconds(100),
                  100,
                  base::Histogram::kUmaTargetedHistogramFlag);
          counter_for_count->AddTime(time_to_paint);
        } else if (render_widget_hosts_loading_.find(render_widget_host) ==
            render_widget_hosts_loading_.end()) {
          got_first_paint_ = true;
        }
      }
      break;
    }
    default:
      NOTREACHED() << "Unknown notification received:" << type;
  }
  if ((got_first_paint_ || render_widget_hosts_to_paint_.empty()) &&
      tabs_loading_.empty() && tabs_to_load_.empty())
    delete this;
}
