function octave_example_simple()
    more off;

    HOST = "localhost";
    PORT = 4223;
    UID = "ncM"; % Change to your UID

    ipcon = java_new("com.tinkerforge.IPConnection"); % Create IP connection
    t = java_new("com.tinkerforge.BrickletTemperature", UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current temperature (unit is °C/100)
    temperature = t.getTemperature();
    fprintf("Temperature: %g°C\n", short2int(temperature)/100.0);

    input("Press any key to exit...\n", "s");
    ipcon.disconnect();
end

function int = short2int(short)
    if compare_versions(version(), "3.8", "<=")
        int = short.intValue();
    else
        int = short;
    end
end
