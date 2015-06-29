function octave_example_threshold()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "ncM"; % Change to your UID

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    t = java_new("com.tinkerforge.BrickletTemperature", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get threshold callbacks with a debounce time of 10 seconds (10000ms)
    t.setDebouncePeriod(10000);

    % Register threshold reached callback to function cb_reached
    t.addTemperatureReachedCallback(@cb_reached);

    % Configure threshold for "greater than 30 °C" (unit is °C/100)
    t.setTemperatureCallbackThreshold(">", 30*100, 0);

    input("Press any key to exit...\n", "s");
    ipcon.disconnect();
end

% Callback for temperature greater than 30 °C
function cb_reached(e)
    fprintf("We have %g°C.\n", short2int(e.temperature)/100.0);
    fprintf("It is too hot, we need air conditioning!\n");
end

function int = short2int(short)
    if compare_versions(version(), "3.8", "<=")
        int = short.intValue();
    else
        int = short;
    end
end

