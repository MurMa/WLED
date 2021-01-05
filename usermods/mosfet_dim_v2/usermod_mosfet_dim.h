#pragma once

#include "wled.h"

//Pin to output the pwm signal to
#ifndef USERMOD_MOSFET_PIN
#define USERMOD_MOSFET_PIN 15 //D8
#endif
//change rate for the pwm value per second, set it to -1 if you don't want a transition
#ifndef USERMOD_TRANSITION_SPEED
#define USERMOD_TRANSITION_SPEED 500
#endif

/*
 * Usermods allow you to add own functionality to WLED more easily
 * See: https://github.com/Aircoookie/WLED/wiki/Add-own-functionality
 * 
 */
//The class for the MosfetDim mod
class MosfetDim : public Usermod {
  private:
    //Private class members. You can declare variables and functions only accessible to your usermod here
    unsigned long lastTime = 0;
    int curPwmValue = 0; //current pwm value
    byte goalPwmValue = 0; //goal pwm value
    byte lastPwmValue = 0; //pwm value before turning wled off (to restore when turning back on)
    boolean powerValue = true; //the current power value of wled
    boolean lastPowerValue = true; //the previous power value before it got changed
    const int updateMs = 10; //milliseconds between updates
    unsigned int changePerUpdate = 0; //per update the curPwmValue will change by this much
  public:
    //Functions called by WLED

    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */
    void setup() {
      //Serial.println("Hello from my usermod!");
      if(USERMOD_TRANSITION_SPEED > 0) {
        changePerUpdate = ceil(USERMOD_TRANSITION_SPEED / (1000 / updateMs));
      }
    }


    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    void connected() {
      //Serial.println("Connected to WiFi!");
    }


    /*
     * loop() is called continuously. Here you can check for events, read sensors, etc.
     * 
     * Tips:
     * 1. You can use "if (WLED_CONNECTED)" to check for a successful network connection.
     *    Additionally, "if (WLED_MQTT_CONNECTED)" is available to check for a connection to an MQTT broker.
     * 
     * 2. Try to avoid using the delay() function. NEVER use delays longer than 10 milliseconds.
     *    Instead, use a timer check as shown here.
     */
    void loop() {
      if(curPwmValue != goalPwmValue) {
        //Update every updateMs milliseconds
        if (millis() - lastTime > updateMs) {
          if(USERMOD_TRANSITION_SPEED <= 0) {
            //No transition
            curPwmValue = goalPwmValue;
          } else {
            //transition until at goal
            if(curPwmValue < goalPwmValue) {
              curPwmValue += changePerUpdate;
              if(curPwmValue > goalPwmValue)
                curPwmValue = goalPwmValue;
            } else if(curPwmValue > goalPwmValue) {
              curPwmValue -= changePerUpdate;
              if(curPwmValue < goalPwmValue)
                curPwmValue = goalPwmValue;
            }
          }
          analogWrite(USERMOD_MOSFET_PIN, curPwmValue);
          lastTime = millis();
        }
      }
    }


    /*
     * addToJsonInfo() can be used to add custom entries to the /json/info part of the JSON API.
     * Creating an "u" object allows you to add custom key/value pairs to the Info section of the WLED web UI.
     * Below it is shown how this could be used for e.g. a light sensor
     */
    void addToJsonInfo(JsonObject& root)
    {
      //this code adds "u":{"Mosfet Dim":[curPwmValue," / 255"]} to the info object
      JsonObject user = root["u"];
      if (user.isNull()) user = root.createNestedObject("u");

      JsonArray lightArr = user.createNestedArray("Mosfet Dim"); //name
      lightArr.add(curPwmValue); //value
      lightArr.add(" / 255"); //unit
    }

    /*
     * addToJsonState() can be used to add custom entries to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void addToJsonState(JsonObject& root)
    {
      root["mosfetdim"] = goalPwmValue;
    }


    /*
     * readFromJsonState() can be used to receive data clients send to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void readFromJsonState(JsonObject& root)
    {
      goalPwmValue = root["mosfetdim"] | goalPwmValue; //if "mosfetdim" key exists in JSON, update, else keep old value
      if(goalPwmValue < 0) Serial.println(F("received invalid mosfet_dim value, cannot be under 0!"));
      if(goalPwmValue > 255) Serial.println(F("received invalid mosfet_dim value, cannot be over 255!"));
      //Check the power state
      powerValue = root["on"] | powerValue;
      if(!powerValue && lastPowerValue) {
      //If Wled is turned off, also set the pwm value to 0
        lastPwmValue = goalPwmValue;
        goalPwmValue = 0;
      } else if(powerValue && !lastPowerValue) {
        //If Wled is turned back on, set pwm value to the value before turning off
        goalPwmValue = lastPwmValue;
      }
      lastPowerValue = powerValue;
    }

    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId()
    {
      return USERMOD_ID_MOSFETDIM;
    }

};