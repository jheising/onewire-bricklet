function matlab_example_simple
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletTemperature;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'nmr'; % Change to your UID
    
    ipcon = IPConnection(); % Create IP connection
    t = BrickletTemperature(UID, ipcon); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current temperature (unit is °C/100)
    temperature = t.getTemperature()/100;

    fprintf('Temperature: %g°C\n', temperature);

    input('\nPress any key to exit...\n', 's');
    ipcon.disconnect();
end

