#  metaTFT
A litte UI package (hardware / software) for building simple list like UIs for µcontroller projects. 

## The Beginning
This project is a spin-off of my [metaPixel](http://github.com/Spirou42/metaPixel) project and was not even planed in the beginning. A couple of years ago a had build a general purpose UI board containing 4 digit 7-segment LED display driven by a [MAX7291](https://www.maximintegrated.com/en/products/power/display-power-control/MAX7219.html) and a rotary encoder with button. 
![OLD UI][olduif] ![OLD UI][olduib]
debouncing (74LS14) + R/C and signal conditioning (1/2 74LS74 used to convert quadrature signal to step/direction) .
I used 2 simple classes one for handling the encoder and one for driving the MAX7291 to create a simple menu system which was able to manipulate *int16_t* data. This was enough for most of my Atmel based projects. 

While working on project [metaPixel](http://github.com/Spirou42/metaPixel) i used more parameters for controlling the than i could handle with this restricted user interface. After i ran out of 7-segment compatible parameter names I started to recreate the user interface.

## Hardware
The new hardware is more flexible and is split up into 3 main parts.

1. small 2.2" [TFT Display](http://ledsee.com/index.php/en/lcd-display/2-2-inch-tft-spi-serial-240-x-320-detail) with SPI interface and SD card reader.
2. [5 keys](http://www.mec.dk/12-26847-Series-details.php?id=289) arranged as a [cursor cross with unmarked caps](http://www.mec.dk/files/MEC/brochures/MEC_ProductCatalogueno7_navimec.pdf) and a legacy [hollow shaft encoder](http://www.mouser.de/Search/ProductDetail.aspx?R=EC60A1520404virtualkey68800000virtualkey688-EC60A1520404)
3. Interface split & Signal conditioning 

The three components / boards are interconnected by standard ribbon cable with [microMaTch](http://www.te.com/deu-de/products/connectors/pcb-connectors/wire-to-board-connectors/ffc-fpc-ribbon-connectors/intersection/micro-match.html?n=125835&type=products&tab=pgp-product) connectors 

### Boards
#### 1. TFT board
The TFT board is a small connector board only holding the ribbon cable connector and the transistor for the backlight control.
![dispb]
![tftani]

#### 2. Key board
![encb]

#### 3. Debouncer
![debb]

## Software


[olduif]: Assets/oldUIfront.jpg "Old UI"
[olduib]: Assets/oldUIback.jpg "Old UI backside"
[encb]: Assets/EncoderBaseSmall.jpg "Encoder Board"
[dispb]: Assets/DisplayBoardSmall.jpg "Display Board"
[debb]: Assets/DebouncerSmall.jpg "Debouncer"
[tftani]: Assets/TFTBoard.mp4 "TFTBoard"
