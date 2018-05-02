function matlab_example_simple()
    import com.tinkerforge.IPConnection;
    import com.tinkerforge.BrickletTemperature;

    HOST = 'localhost';
    PORT = 4223;
    UID = 'XYZ'; % Change XYZ to the UID of your Temperature Bricklet

    ipcon = IPConnection(); % Create IP connection
    t = handle(BrickletTemperature(UID, ipcon), 'CallbackProperties'); % Create device object

    ipcon.connect(HOST, PORT); % Connect to brickd
    % Don't use device before ipcon is connected

    % Get current temperature (unit is °C/100)
    temperature = t.getTemperature();
    fprintf('Temperature: %g °C\n', temperature/100.0);

    input('Press key to exit\n', 's');
    ipcon.disconnect();
end
