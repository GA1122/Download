void GetReceiversAndActivitiesCallback(
    const ash::CastConfigDelegate::ReceiversAndActivitesCallback& callback,
    const base::Value* value) {
  ash::CastConfigDelegate::ReceiversAndActivites receiver_activites;
  const base::ListValue* ra_list = nullptr;
  if (value->GetAsList(&ra_list)) {
    for (auto i = ra_list->begin(); i != ra_list->end(); ++i) {
      const base::DictionaryValue* ra_dict = nullptr;
      if ((*i)->GetAsDictionary(&ra_dict)) {
        const base::DictionaryValue* receiver_dict(nullptr),
            *activity_dict(nullptr);
        ash::CastConfigDelegate::ReceiverAndActivity receiver_activity;
        if (ra_dict->GetDictionary("receiver", &receiver_dict)) {
          receiver_dict->GetString("name", &receiver_activity.receiver.name);
          receiver_dict->GetString("id", &receiver_activity.receiver.id);
        }
        if (ra_dict->GetDictionary("activity", &activity_dict) &&
            !activity_dict->empty()) {
          activity_dict->GetString("id", &receiver_activity.activity.id);
          activity_dict->GetString("title", &receiver_activity.activity.title);
          activity_dict->GetString("activityType",
                                   &receiver_activity.activity.activity_type);
          activity_dict->GetBoolean("allowStop",
                                    &receiver_activity.activity.allow_stop);
          activity_dict->GetInteger("tabId",
                                    &receiver_activity.activity.tab_id);
        }
        receiver_activites[receiver_activity.receiver.id] = receiver_activity;
      }
    }
  }
  callback.Run(receiver_activites);
}
