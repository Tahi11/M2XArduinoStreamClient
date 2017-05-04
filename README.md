Arduino M2X API Client
=====================

**NOTE**: As of version 2.2.0, we have change the library to header-only implementation. To read more about the reason behind this change, please check out [RATIONALE](./RATIONALE.md) file.

The Arduino library is used to send/receive data to/from [AT&amp;T's M2X service](https://m2x.att.com/) from [Arduino](http://www.arduino.cc/) based devices.

This library has been tested on the following embedded boards:

* Arduino Uno
* Arduino Yun
* LinkIt One
* ESP8266

If you are using other boards, your mileage may very.

Getting Started
==========================
1. Signup for an [M2X Account](https://m2x.att.com/signup).
2. Obtain your _Master Key_ from the Master Keys tab of your [Account Settings](https://m2x.att.com/account) screen.
3. Create your first [Device](https://m2x.att.com/devices) and copy its _Device ID_.
4. Review the [M2X API Documentation](https://m2x.att.com/developer/documentation/overview).
5. Obtain an Arduino with built-in wifi or ethernet, or a separate wifi or ethernet shield and [set it up](http://arduino.cc/en/Guide/HomePage). These docs were written for an [__Arduino Uno__](http://arduino.cc/en/Main/arduinoBoardUno) with a wifi or ethernet shield but the instructions can be adapted for other Arduino models.

Please consult the [M2X glossary](https://m2x.att.com/developer/documentation/glossary) if you have questions about any M2X specific terms.

How to Install the library
==========================

This library depends on [jsonlite](https://github.com/citrusbyte/jsonlite), the installation steps are as follows:

1. Clone the repository and sync submodules:

   ```
   $ git clone https://github.com/attm2x/m2x-arduino
   $ cd m2x-arduino
   $ git submodule update --init
   ```

   **NOTE**: You might notice that we add jsonlite as a submodule, this is due to the fact that we are still using an older version of jsonlite now. We might remove this restriction once we migrate to latest version.

2. Open the Arduino IDE, click `Sketch->Import Library...->Add Library...`, then navigate to `vendor/jsonlite/amalgamated/jsonlite` folder in the repository. The jsonlite library will be imported to Arduino this way.

   **NOTE**: There will be three (3) folders named jsonlite:
   * `vendor/jsonlite`: the repo folder
   * `vendor/jsonlite/jsonlite`: the un-flattened jsonlite source folder
   * `vendor/jsonlite/amalgamated/jsonlite`: the flattened jsonlite source for arduino

   You should use the final library listed here as the first two won't work!
3. Use the instructions outlined in Step 2 above to import the `M2XStreamClient` library in the repository.
4. Now you can find M2X examples under `File->Examples->M2XStreamClient`

Hardware Setup
==============

Board Setup
-----------

The Arduino website has a very good [tutorial](http://arduino.cc/en/Guide/HomePage) on setting up the Arduino board. It contains detailed instructions on how to install the Arduino IDE, sets up your board for initial testing. Feel free to proceed to the [Arduino site](http://arduino.cc/en/Guide/HomePage) to get a basic idea on Arduino.

Wifi/Ethernet Shield Setup
--------------------------

To send data to the AT&amp;T M2X service, or receive data from the AT&amp;T M2X service, your Arduino board needs a connection to the Internet. Hence, an Arduino [Wifi Shield](http://arduino.cc/en/Main/ArduinoWiFiShield) or [Ethernet Shield](http://arduino.cc/en/Main/ArduinoEthernetShield) might be needed to give your board the power to connect to the Internet. To install the shield, hook the shield onto your Arduino board — you can use the pins on the shield the same way as the real pins on the Arduino boards.

However, if you are using an [Arduino Yun](http://arduino.cc/en/Main/ArduinoBoardYun) board, [LinkIt One board](http://www.seeedstudio.com/depot/LinkIt-ONE-p-2017.html) or [ESP8266 module](http://www.adafruit.com/product/2471) instead of an [Arduino Uno](http://arduino.cc/en/Main/ArduinoBoardUno) board, you can skip this section since these boards normally has Wifi or Ethernet adapter on board, however, it is recommended to check out the board's manual for detailed instructions.

Sensor Setup
------------

Different sensors can be hooked up to an Arduino board to provide different properties including temperatures, humidity, etc. You can use a breadboard as well as wires to connect different sensors to your Arduino. For a detailed tutorial on connecting different sensors, please refer to the Arduino [Examples page](http://arduino.cc/en/Tutorial/HomePage).

3G Shield Setup
------------

If you are using a 3G GSM/GPRS shield with your Arduino, the [M2X Arduino SIMCOM module](https://github.com/attm2x/m2x-arduino-simcom) can be used to connect your device to the AT&T cellular data network.


Variables used in Examples
==========================

In order to run the given examples, different variables need to be configured. We will walk through those variables in this section.

Network Configuration
---------------------

If you are using a Wifi Shield, the following variables need configuration:

```
char ssid[] = "<ssid>";
char pass[] = "<WPA password>";
```

Just fill in the SSID and password of the Wifi hotspot and you should be good to go.

For an Ethernet Shield, the following variables are needed:

```
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192,168,1,17);
```

For a newer Ethernet Shield, the MAC address should be printed on a sticker on the shield. However, some old Ethernet Shields have no MAC address shown on the board. In this case, you can use any MAC address, as long as it does not conflict with another network device within the same LAN.

The IP address here is only used when DHCP fails to give a valid IP address. It is recommended, though not required, to provide a unique IP address.

M2X API Key
-----------

Once you [register](https://m2x.att.com/signup) for an AT&amp;T M2X account, an API key is automatically generated for you. This key is called a _Primary Master Key_ and can be found in the _Master Keys_ tab of your [Account Settings](https://m2x.att.com/account). This key cannot be edited or deleted, but it can be regenerated. It will give you full access to all APIs.

However, you can also create a _Device API Key_ associated with a given Device, you can use the Device API key to access the streams belonging to that Device.

You can customize this variable in the following line in the examples:

```
char m2xKey[] = "<M2X access key>";
```

Device ID
-------

A device is a source of data (it could be a physical device, a virtual device, a service, or an application). It is a set of data streams, such as streams of locations, temperatures, etc. The following line is needed to configure the device used:

```
char deviceId[] = "<device id>";
```

Stream Name
------------

A stream in a device is a set of times series data of a specific type (i,e. humidity, temperature). You can use the M2XStreamClient library to send stream values to M2X server, or receive stream values from M2X server. Use the following line to configure the stream if needed:

```
char streamName[] = "<stream name>";
```

Using the M2XStreamClient library
=========================

To use the M2X Arduino library, you need to import the library first:

```
#define ARDUINO_PLATFORM
#include <M2XStreamClient.h>
```

Notice as of this version of the library, you have to define the board platform you will use. We might add auto-detection part in the future, but let's leave that for a future implementation. Supported platform right now include:

* `ARDUINO_PLATFORM`: can be used for Arduino boards(Uno, Yun, etc.) as well as LinkIt One boards.
* `ESP8266_PLATFORM`: can be used for ESP8266 boards.

These 2 lines right here only defines the writer functions, in other words, functions you can use to send values to M2X. To use reader functions, you have to add 2 more lines:

```
#define ARDUINO_PLATFORM
#define M2X_ENABLE_READER
#include <jsonlite.h>
#include <M2XStreamClient.h>
```

This will give you reader functions so you can read M2X values from your board. However, keep in mind that reader functions are only available for certain boards. One example is that as of now, ESP8266 platform does not support reader functions.

The M2X Arduino library can be used with both a Wifi connection and an Ethernet connection. For a Wifi connection, use the following code:

```
WiFiClient client;
M2XStreamClient m2xClient(&client, m2xKey);
```

For an Ethernet connection, use the following code:

```
EthernetClient client;
M2XStreamClient m2xClient(&client, m2xKey);
```

If you are using an [Arduino Yun](http://arduino.cc/en/Main/ArduinoBoardYun) board, you should use the following code:

```
YunClient client;
M2XStreamClient m2xClient(&client, m2xKey);
```

Foe LinkIt One boards, use the following Wifi client specification:

```
LWifiClient client;
M2XStreamClient m2xClient(&client, m2xKey);
```

For other boards not listed here, you might want to check their manual for the correct client type to use. Or you can also refer to the examples we provided for some tips.

In the M2XStreamClient, the following writer API functions are provided:

* `updateStreamValue`: Send stream value to M2X server
* `postDeviceUpdates`: Post values from multiple streams to M2X server
* `updateLocation`: Send location value of a device to M2X server
* `deleteValues`: Delete stream values from M2X server

And the following 2 reader functions are provided:

* `listStreamValues`: Receive stream value from M2X server
* `readLocation`: Receive location values of a device from M2X server

Returned values
---------------

For all those functions, the HTTP status code will be returned if we can fulfill an HTTP request. For example, `200` will be returned upon success, and `401` will be returned if we didn't provide a valid M2X API Key. A full-list of M2X API error codes can be found here: [M2X API Error Codes] (https://m2x.att.com/developer/documentation/overview#Client-Errors)

Otherwise, the following error codes will be used:

```
static const int E_NOCONNECTION = -1;
static const int E_DISCONNECTED = -2;
static const int E_NOTREACHABLE = -3;
static const int E_INVALID = -4;
static const int E_JSON_INVALID = -5;
```

Update stream value
-------------------

The following functions can be used to post one single value to a stream, which belongs to a device:

```
template <class T>
int updateStreamValue(const char* deviceId, const char* streamName, T value);
```

Here we use C++ templates to generate functions for different types of values, feel free to use values of `float`, `int`, `long` or even `const char*` types here.

Post device updates
-------------------

M2X also supports posting multiple values to multiple streams in one call, use the following function for this:

```
template <class T>
int postDeviceUpdates(const char* deviceId, int streamNum,
                      const char* names[], const int counts[],
                      const char* ats[], T values[]);
```

Please refer to the comments in the source code on how to use this function, basically, you need to provide the list of streams you want to post to, and values for each stream.

Update Device Location
--------------------------

You can use the following function to update the location for a device:

```
template <class T>
int updateLocation(const char* deviceId, const char* name,
                   T latitude, T longitude, T elevation);
```

Different from stream values, locations are attached to devices rather than streams. We use templates here, since the values may be in different format, for example, you can express latitudes in both `double` and `const char*`.

Delete stream values
--------------------

The following function can be used to delete stream values within a date range:

```
int deleteValues(const char* deviceId, const char* streamName,
                 const char* from, const char* end);
```

`from` and `end` fields here follow ISO 8601 time format.

List stream values
------------------

Since Arduino microcontrollers contain very limited memory, we cannot put the whole returned string in memory, parse it into JSON representations and read what we want. Instead, we use a callback-based mechanism here. We parse the returned JSON string piece by piece, whenever we got a new stream value point, we will call the following callback functions:

```
typedef void (*stream_value_read_callback)(const char* at,
                                           const char* value,
                                           int index,
                                           void* context,
                                           int type);
```

The implementation of the callback function is left for the user to fill in, you can read the value of the point in the `value` argument, and the timestamp of the point in the `at` argument. We even pass the index of this this data point in the whole stream as well as a user-specified context variable to this function, so as you can perform different tasks on this.

`type` indicates the type of value stored in `value`: 1 for string, 2 for number. However, keep in mind that `value` will always be a pointer to an array of char, even though `type` indicates the current value is a number. In this case, `atoi` or `atof` might be needed.

To read the stream values, all you need to do is calling this function:

```
int listStreamValues(const char* deviceId, const char* streamName,
                     stream_value_read_callback callback, void* context,
                     const char* query = NULL);
```

Besides the device ID and stream name, only the callback function and a user context needs to be specified. Optional query parameters might also be available here, for example, the current query parameter picks value from a specific range:

```
start=2014-10-01T00:00:00Z&end=2014-10-10T00:00:00Z
```

Read Device Location
------------------------

Similar to reading stream values, we also use callback functions here. The only difference is that different parameters are used in the function:

```
void (*location_read_callback)(const char* name,
                               double latitude,
                               double longitude,
                               double elevation,
                               const char* timestamp,
                               int index,
                               void* context);

```

For memory space consideration, now we only provide double-precision when reading locations. An index of the location points is also provided here together with a user-specified context.

The API is also slightly different, in that the stream name is not needed here:

```
int readLocation(const char* deviceId, location_read_callback callback,
                 void* context);

```

Examples
========

We provide a series of examples that will help you get an idea of how to use the `M2XStreamClient` library to perform all kinds of tasks.

The name of each example consists of 2 parts: the board to use, and the function of that example. For example, `UnoPost` means this example targets the Arduino Uno board with a Wifi adapter, and the example will post a value to M2X. `EthernetUnoReceive` means this example also targets the Arduino Uno board, but uses an Ethernet adapter, the example will fetch values from M2X.

Note that the examples contain fictionary variables, and that they need to be configured as per the instructions above before running on your Arduino board. Each of the examples here also needs either a Wifi Shield or an Ethernet Shield hooked up to your device.

In the `UnoPost`, `EthernetUnoPost` and `YunPost`, a temperature sensor, a breadboard and 5 wires are also needed to get temperature data, and you will need to wire the board like [this](http://cl.ly/image/3M0P3T1A0G0l) before running the code. If you are using other boards, keep in mind that we are reading from `A0` in the code and the wiring should be similar to this one shown in the illustration.

After you have configured your variables and the board, plug the Arduino board into your computer via a Micro-USB cable, click `Verify` in the Arduino IDE, then click `Upload`, and the code should be uploaded to the board. You can check all the outputs in the `Serial Monitor` of the Arduino IDE.

Below is a short description for each example function group:

Post
-------

This example shows how to post temperatures to M2X.

Time
-------

This example works like `Post`, except that it uses M2X Time API with the TimeService provided to generate timestamps for the posted data.

PostMultiple
---------------

This example shows how to post multiple values to multiple streams in one API call.

UpdateLocation
-----------------

This example sends location data to M2X. Ideally a GPS device should be used here to read the coordinates, but for simplicity, we just use pre-set values here to show how to use the API.

Delete
---------

This example shows how to delete values within a stream by providing a date/time range.

FetchValues
--------------

This example reads stream values from M2X server and prints the stream data point to Serial interface. You can find the actual values in the Arduino `Serial Monitor`.

ReadLocation
---------------

This example reads location data of a device from M2X, and prints them to Serial interface. You can check the output in the `Serial Monitor` of the Arduino IDE.

LinkIt One Board Note
=====================

To run the examples for a LinkIt One board, The steps are slightly different from a standard Arduino board, and are listed below:

* Follow the [Quick Start Guide](http://www.seeedstudio.com/wiki/LinkIt_ONE#Quick_Start_Guide) to setup the environment for the board. Keep a note of the 2 COM ports used as we will use both of them later.
* Follow the `How to Install the library` section above to install the libraries needed.
* Open an example for LinkIt One board, configure the variables, including M2X key, Device ID, Stream Name, values, etc.
* From the Arduino IDE menu, click on _Tools_, _Board_, then select `LinkIt ONE` as the board to use.
* Also from the Arduino IDE menu, click on _Tools_, _Port_, then select the COM port which is marked as Debug Port in Step 1.
* Click _Upload_ to build and upload the examples.
* From the Arduino IDE menu, click on _Tools_, _Port_ then select the COM port which is marked as Modem Port in Step 1.
* Open the _Serial Monitor_, type anything in the input line above, then hit `Send`. The program is configured to start running only at this point, because you might otherwise miss the Serial outputs.

*NOTE*: Currently LinkIt One board development is only supported on Microsoft Windows XP, Windows Vista and Windows 7. It is strongly recommended that you use a 32-bit version instead of a 64-bit version for maximum compatibility.

ESP8266 Note
============

We've successfully tested the example on an ESP8266 chip provided in the [Adafruit HUZZAH ESP8266 Breakout](http://www.adafruit.com/products/2471?&main_page=product_info&products_id=2471). If you are using other ESP8266 breakout combination, the code will theoretically work, but the setup step might be slightly different.

To run the example, you should follow the setup guide at [here](https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/overview) to assemble the board. Then you can install the M2X library following notes above, at this time, you should be able to start playing with the board.

However, 2 points here are worth mentioning:

* ESP8266 chip must be put into a special bootload mode before you flashing the Arduino program onto the chip, otherwise, the steps will just fail with `espcomm_open failed` error. To turn the chip into this mode, you have to follow the steps at [here](https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/using-arduino-ide#blink-test). Notice this only applies to Adafruit HUZZAH ESP8266 Breakout, if you are using other modules, the steps may vary.
* Right now only writer functions work on ESP8266 chip, in other words, you can use the board to post values to M2X, but not read and parse value returned by M2X server. This is because we haven't found a way to get jsonlite compiled successfully on ESP8266. We will continue working on this and will change this note if/when we can get it working.

LICENSE
=======

This library is released under the MIT license. See [`M2XStreamClient/LICENSE`](M2XStreamClient/LICENSE) for the terms.
