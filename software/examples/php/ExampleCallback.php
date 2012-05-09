<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletTemperature.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletTemperature;

$host = 'localhost';
$port = 4223;
$uid = 'XYZ'; // Change to your UID

// Callback function for temperature callback (parameter has unit °C/100)
function cb_temperature($temperature)
{
    echo "Temperature: " . $temperature / 100.0 . " °C\n";
}

$ipcon = new IPConnection($host, $port); // Create IP connection to brickd
$t = new BrickletTemperature($uid); // Create device object

$ipcon->addDevice($t); // Add device to IP connection
// Don't use device before it is added to a connection

// Set Period for temperature callback to 1s (1000ms)
// Note: The callback is only called every second if the 
//       temperature has changed since the last call!
$t->setTemperatureCallbackPeriod(1000);

// Register illuminance callback to function cb_temperature
$t->registerCallback(BrickletTemperature::CALLBACK_TEMPERATURE, 'cb_temperature');

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
