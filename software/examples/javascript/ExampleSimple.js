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
        // Get current temperature (unit is °C/100)
        t.getTemperature(
            function(temp) {
                console.log('Temperature: '+temp/100+' °C');
            },
            function(error) {
                console.log('Error: '+error);
            }
        );
    }
);

console.log("Press any key to exit ...");
process.stdin.on('data',
    function(data) {
        ipcon.disconnect();
        process.exit(0);
    }
);

