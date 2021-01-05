# Mosfet dimming

This usermod will output a pwm signal to a pin. This can be used to control and dim an analog led.
The pwm value can be set via the JSON API or by adjusting a slider in the Web UI.

## Installation 

### JSON API functionality

Copy the example `platformio_override.ini` to the root directory.  This file should be placed in the same directory as `platformio.ini`.

Replace the `wled00/src/usermods_list.cpp` file with the one in this directory.

### Controlling via GUI 

If you want to be able to control the mosfet dim value using the gui, you need to make changes in two files:

### In `index.htm`

You can replace the wled00/data/index.htm file with the one in this directory or do the following changes manually:

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
            <output class="sliderbubble hidden"></output>
            <div class="sliderdisplay"></div>
        </div>
    </div>
</div>
```


### In `index.js`

You can replace the wled00/data/index.js file with the one in this directory or do the following changes manually:

`Add`
```
function setMosfetDim() {
	var obj = {"mosfetdim": parseInt(d.getElementById('sliderMosfetDim').value)};
	requestJson(obj, false);
}
```
anywhere in index.js (I put it beneath the setIntensity() function)

`Add`
```
obj.md = parseInt(d.getElementById('sliderMosfetDim').value);	
```
in the saveP() function:
```
function saveP(i) {
    ...
    if (!d.getElementById(`p${i}cstgl`).checked) {
        ...
        obj.o = true;
    } else {
        obj.ib = d.getElementById(`p${i}ibtgl`).checked;
        obj.sb = d.getElementById(`p${i}sbtgl`).checked;
    --> obj.mosfetdim = parseInt(d.getElementById('sliderMosfetDim').value); <--
    }
    obj.psave = pI; obj.n = pN;
    ...
}
```

`Add`
```
d.getElementById('sliderMosfetDim').value= s.mosfetdim;
```
in the requestJson() function:
```
function requestJson(command, rinfo = true, verbose = true) {
    ...
    isOn = s.on;
    d.getElementById('sliderBri').value= s.bri;
--> d.getElementById('sliderMosfetDim').value= s.mosfetdim; <--
    nlA = s.nl.on;
    nlDur = s.nl.dur;
    ...
}
```

`Add`
```
updateTrail(d.getElementById('sliderMosfetDim'));
```
in the updateUI() function:
````
function updateUI() {
    ...
    updateTrail(d.getElementById('sliderW'));
--> updateTrail(d.getElementById('sliderMosfetDim')); <--
	if (isRgbw) d.getElementById('wwrap').style.display = "block";
    ...
}
```


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