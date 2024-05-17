  OpenChannelParams(content::RenderProcessHost* source,
                    const std::string& tab_json,
                    MessagePort* receiver,
                    int receiver_port_id,
                    const std::string& source_extension_id,
                    const std::string& target_extension_id,
                    const std::string& channel_name)
      : source(source),
        tab_json(tab_json),
        receiver(receiver),
        receiver_port_id(receiver_port_id),
        source_extension_id(source_extension_id),
        target_extension_id(target_extension_id),
        channel_name(channel_name) {}
