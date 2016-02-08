# saltlamp-arduino

This is a piece of software that is somewhat similar to MySensors. However, it does not work wirelessly. It leverages the USB-serial connection on the Arduinos. It also acts like an _I can do it everything_ device. This means that you upload this firmware to your Arduino and then set up your sensors over the serial connection.

You can use the code from the `saltlamp-arduino-mqtt` repo in order to connect your sensors to an MQTT network.

The idea behind all this is that you:

1. take an Arduino
2. upload this firmware as it is
3. hook up your sensors
4. connect the Arduino to an OpenWRT router or a SBC (single-board computer) or whatever that is a low power device
5. download `saltlamp-arduino-mqtt` and in the configuration file you actually specify what devices are connected to your Arduino
6. run the thing and you have your IoT devices on the MQTT network!

For the documentation of the protocol, see the `DOCS.md` file or respective source codes.
