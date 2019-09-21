
# TODO

## 1. Configuration over MQTT

Settings over the topic `doorbell/set`. The payload is a JSON document:

```json
{
    "unmute_if_no_wifi" : 1,
    "mute" : 0,
    "button_held_ms" : 200,
}
```

 * `unmute_if_no_wifi` - In case if there is no connection the relay will be ON whenever the button is pressed even if the doorbell was muted.
 * `mute` - Send only an mqtt message but the relay won't be turned on
 * `button_held_ms` - for how long the relay will be held ON when the button is pressed.


## 2. Save configuration

 The configuration parameters shall be saved in EEPROM

## 3.
