# Shelly Rotary dimmer


This is a quick solution I came up with for someone who wants a rotary dimmer to use with a **shelly dimmer** (or any MQTT-enabled dimmer).
Sometimes you really want to "turn a button" to dim the lights.

You can finish it however you want. The easiest would be to make a wooden base (I think 20mm thickness should be enough) and put in the electronics. Then attach a button (can again be made of wood, maybe 3-4cm thickness) to the top of the potentiometer.

If you're handy and feel like making a really cool project, check out this "DIY Perks" video:

https://www.youtube.com/watch?v=sJ5vhShdVjo

This would give you a really, really cool dimmer to place on a table or windowsill.


# Assumptions

To use/make this project I'm going to assume a few things.
- You use shelly in MQTT mode. This is reliant on MQTT communication.
- This implies that you **will** lose cloud connection!

# Item list (what we need)

- Wemos D1 mini (easily found on [aliexpress](https://nl.aliexpress.com/item/32255861885.html?spm=a2g0s.9042311.0.0.27424c4d8fnswV))
- A potentiometer (also easily found on [aliexpress](https://nl.aliexpress.com/item/4000102315830.html?spm=a2g0s.9042311.0.0.27424c4dRxdJf8)
- Some wire
- 5V power supply to your dimmer module (I usually repurpose an old phone charger)
- Usb cable to power the Wemos D1 mini.


# Creating our Dimmer

Creating the setup is pretty simple, as we only need 3 wires:
- One extremity wire from the potentiometer to 5V on the wemos D1 mini
- Other extremity wire from the potentiometer to GND on the wemos D1 mini.
- The middle wire goes to A0 on the wemos.

Schematic using a NodeMCU (this uses 3V3, I use 5V):

![Example schematic](https://i.imgur.com/rk3a9ui.jpg)

## Setting this up

### Potentiometer.ino
- Change mqtt_debug to something else. This is just a "ping" message sent.
Copy arduino_secrets_ex.h to arduino_secrets.h , change the values to something applicable to you.

### Arduino_secrets_ex.h
Copy arduino_secrets_ex.h to arduino_secrets.h , change the relevant settings.


## Testing

After uploading the sketch to your Wemos, you can subscribe to the mqtt channel you provided, and see results!

![sample output](https://i.imgur.com/zDJd4uy.jpg)

If you used the correct shelly device ID, it should already be dimming your lights!
