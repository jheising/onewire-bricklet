var IPConnection = require('Tinkerforge/IPConnection');
var BrickletTemperature = require('Tinkerforge/BrickletTemperature');

var HOST = 'localhost';
var PORT = 4223;
var UID = 'dUh';// Change to your UID

var ipcon = new IPConnection();// Create IP connection
var t = new BrickletTemperature(UID, ipcon);// Create device object

ipcon.connect(HOST, PORT,
    function(error) {
        console.log('Error: '+error);        
    }
);// Connect to brickd

// Don't use device before ipcon is connected
ipcon.on(IPConnection.CALLBACK_CONNECTED,
    function(connectReason) {
        // Set threshold callbacks with a debounce time of 10 seconds (10000ms)
        t.setDebouncePeriod(10000);
        // Configure threshold for "greater than 30 °C" (unit is °C/100)
        t.setTemperatureCallbackThreshold('>', 30*100, 0);       
    }
);

// Register threshold reached callback
t.on(BrickletTemperature.CALLBACK_TEMPERATURE_REACHED,
    // Callback for temperature greater than 30 °C
    function(temp) {
        console.log('We have '+temp/100.0+' °C.');
        console.log('It is too hot, we need air conditioning!');
        console.log();
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

