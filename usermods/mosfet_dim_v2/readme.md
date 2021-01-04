# Mosfet dimming

This usermod will output a pwm signal to a pin. This can be used to control and dim an analog led.
The pwm value can be set via the JSON API or by adjusting a slider in the Web UI.

## Installation 

### JSON API functionality

Copy the example `platformio_override.ini` to the root directory.  This file should be placed in the same directory as `platformio.ini`.

### Controlling via GUI 

If you want to be able to control the mosfet dim value using the gui, you need to make two changes:

### In `index.htm`

Add

```
<div id="briwrap">
    <p class="hd">Mosfet Dim</p>
    <div class="il">
        <i class="icons slider-icon">&#xe2a6;</i>
        <div class="sliderwrap il">
            <input id="sliderMosfetDim" onchange="setMosfetDim()" oninput="updateTrail(this)" max="255" min="0" type="range" value="0" />
            <div class="sliderdisplay"></div>
        </div>
    </div>
</div>
```
after this div: 
```
<div id="briwrap">
    <p class="hd">Brightness</p>
    <div class="il">
        <i class="icons slider-icon" onclick="tglTheme()">&#xe2a6;</i>
        <div class="sliderwrap il">
            <input id="sliderBri" onchange="setBri()" oninput="updateTrail(this)" max="255" min="1" type="range" value="128" />
            <div class="sliderdisplay"></div>
        </div>
    </div>
</div>
```


### In `index.js`

Add

```
function setMosfetDim() {
	var obj = {"mosfetdim": parseInt(d.getElementById('sliderMosfetDim').value)};
	requestJson(obj, false);
}
```
under the setIntensity() function

### Compilation

To recompile the Web UI you need to run:
```
npm install
```
and 
```
npm run build
```
See [`Changing Web UI`](https://github.com/Aircoookie/WLED/wiki/Add-own-functionality#changing-web-ui)

then proceed with the usual compilation as described [here](https://github.com/Aircoookie/WLED/wiki/Compiling-WLED)


## Usage

To control the pwm value, you can use the JSON API. Send the key "mosfetdim" and a value from 0 to 255.

Example for half brightness (technically, not perceived): `{"mosfetdim":128}`

You can view the current mosfet dim value in the Info screen in the GUI or using the JSON API.

A slider in the Web UI can be used to control the brightness.