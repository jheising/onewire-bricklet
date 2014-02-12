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
        // Set Period for temperature callback to 1s (1000ms)
        // Note: The callback is only called every second if the 
        // temperature has changed since the last call!
        t.setTemperatureCallbackPeriod(1000);       
    }
);

// Register temperature callback
t.on(BrickletTemperature.CALLBACK_TEMPERATURE,
    // Callback function for temperature callback (parameter has unit °C/100)
    function(temp) {
        console.log('Temperature: '+temp/100+' °C');
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

