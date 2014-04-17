function octave_example_callback()
    more off;
    
    HOST = "localhost";
    PORT = 4223;
    UID = "nmr"; % Change to your UID

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    t = java_new("com.tinkerforge.BrickletTemperature", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Set Period for temperature callback to 1s (1000ms)
    % Note: The callback is only called every second if the 
    %       temperature has changed since the last call!
    t.setTemperatureCallbackPeriod(1000);

    % Register temperature callback to function cb_temperature
    t.addTemperatureCallback(@cb_temperature);
    
    input("Press any key to exit...\n", "s");
    ipcon.disconnect();
end

% Callback function for temperature callback (parameter has unit °C/100)
function cb_temperature(e)
    fprintf("Temperature: %g°C\n", str2num(e.temperature.toString())/100.0);
end
