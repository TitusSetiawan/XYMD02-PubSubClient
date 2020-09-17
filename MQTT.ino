void set_MqttServer() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[ANTARES] Unable to connect to MQTT server.\n");
  }
  else {
    Serial.println("[ANTARES] Setting MQTT server \"" + String(_mqttServer) + "\" on port " + String(_mqttPort) + "\n");
    client.setServer(_mqttServer, _mqttPort);
  }

}

void check_MqttConnection()
{
  _subscriptionTopic = "/oneM2M/resp/antares-cse/" + _accessKey + "/json";

  if (!client.connected()) {
    while (!client.connected()) {
      Serial.println("[ANTARES] Attempting MQTT connection...\n");

      String clientId = "ESP32-" + _accessKey;

      char clientIdChar[clientId.length() + 1];
      clientId.toCharArray(clientIdChar, clientId.length() + 1);

      if (client.connect(clientIdChar)) {
        Serial.println("[ANTARES] Connected! Client ID:");
        Serial.print(clientIdChar);
        Serial.print("\n");
        char subscriptionTopicChar[_subscriptionTopic.length() + 1];
        _subscriptionTopic.toCharArray(subscriptionTopicChar, _subscriptionTopic.length() + 1);

        Serial.println();
        Serial.print("[ANTARES] Subscribe Topic: ");
        Serial.println(subscriptionTopicChar);

        client.publish(subscriptionTopicChar, "connect!");
        client.subscribe(subscriptionTopicChar);
      }
      else {
        Serial.println("[ANTARES] Failed, rc=" + String(client.state()) + ", Will try again in 5 secs.\n");
        delay(5000);
      }
    }
  }
  client.loop();
}

void addData(String key, float value) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& object = jsonBuffer.parseObject(_jsonDataString);
  object[key] = value;
  String newInsert;
  object.printTo(newInsert);
  _jsonDataString = newInsert;
  //  Serial.println(_jsonDataString);
}

void publishData(String your_projectName, String your_deviceName) {
  String topic = "/oneM2M/req/" + _accessKey + "/antares-cse/json";
  String finalData;

  if (_debug) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& object = jsonBuffer.parseObject(_jsonDataString);
    Serial.print("[DEBUG ON] [ANTARES] Publish Topic: "); Serial.println(topic);
    Serial.print("[DEBUG ON] [ANTARES] PUBLISH DATA:\n\n");
    object.prettyPrintTo(Serial);
    Serial.println("\n");
  }

  _jsonDataString.replace("\"", "\\\"");


  finalData += "{";
  finalData += "\"m2m:rqp\": {";
  finalData += "\"fr\": \"" + _accessKey + "\",";
  finalData += "\"to\": \"/antares-cse/antares-id/" + your_projectName + "/" + your_deviceName + "\",";
  finalData += "\"op\": 1,";
  finalData += "\"rqi\": 123456,";
  finalData += "\"pc\": {";
  finalData += "\"m2m:cin\": {";
  finalData += "\"cnf\": \"message\",";
  finalData += "\"con\": \"" + _jsonDataString + "\"";
  finalData += "}";
  finalData += "},";
  finalData += "\"ty\": 4";
  finalData += "}";
  finalData += "}";

//////// uncomment to view json //////////
//   DynamicJsonBuffer jsonBuffer2;
//   JsonObject& object2 = jsonBuffer2.parseObject(finalData);
//   object2.prettyPrintTo(Serial);

  char finalDataChar[finalData.length() + 1];
  char topicChar[topic.length() + 1];

  finalData.toCharArray(finalDataChar, finalData.length() + 1);
  topic.toCharArray(topicChar, topic.length() + 1);

  _jsonDataString = "{}";

//  Serial.println(finalDataChar);
  cekPub = client.publish(topicChar, finalDataChar);
  if(cekPub == true)
  {
    Serial.println("Publish Sukses ^_^  status client.publish() = true");
  }
}
