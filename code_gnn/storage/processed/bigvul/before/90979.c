		void CWebServer::Cmd_DeleteHardware(WebEmSession & session, const request& req, Json::Value &root)
		{
			if (session.rights != 2)
			{
				session.reply_status = reply::forbidden;
				return;  
			}

			std::string idx = request::findValue(&req, "idx");
			if (idx.empty())
				return;
			int hwID = atoi(idx.c_str());

			CDomoticzHardwareBase *pBaseHardware = m_mainworker.GetHardware(hwID);
			if ((pBaseHardware != NULL) && (pBaseHardware->HwdType == HTYPE_DomoticzInternal)) {
				return;
			}

			root["status"] = "OK";
			root["title"] = "DeleteHardware";

			m_mainworker.RemoveDomoticzHardware(hwID);
			m_sql.DeleteHardware(idx);
		}
