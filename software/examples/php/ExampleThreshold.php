<?php

require_once('Tinkerforge/IPConnection.php');
require_once('Tinkerforge/BrickletTemperature.php');

use Tinkerforge\IPConnection;
use Tinkerforge\BrickletTemperature;

$host = 'localhost';
$port = 4223;
$uid = 'XYZ'; // Change to your UID

// Callback for temperature greater than 30 °C
function cb_reached($temperature)
{
    echo "We have " . $temperature / 100.0 . " °C.\n";
    echo "It is too hot, we need air conditioning!\n";
}

$ipcon = new IPConnection($host, $port); // Create IP connection to brickd
$t = new BrickletTemperature($uid); // Create device object

$ipcon->addDevice($t); // Add device to IP connection
// Don't use device before it is added to a connection

// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
$t->setDebouncePeriod(10000);

// Register threshold reached callback to function cb_reached
$t->registerCallback(BrickletTemperature::CALLBACK_TEMPERATURE_REACHED, 'cb_reached');

// Configure threshold for "greater than 30 °C" (unit is °C/100)
$t->setTemperatureCallbackThreshold('>', 30*100, 0);

echo "Press ctrl+c to exit\n";
$ipcon->dispatchCallbacks(-1); // Dispatch callbacks forever

?>
