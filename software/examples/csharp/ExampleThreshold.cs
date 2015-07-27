using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "XYZ"; // Change to your UID

	// Callback function for temperature greater than 30 °C (parameter has unit °C/100)
	static void TemperatureReachedCB(BrickletTemperature sender, short temperature)
	{
		System.Console.WriteLine("Temperature: " + temperature/100.0 + " °C");
		System.Console.WriteLine("It is too hot, we need air conditioning!");
	}

	static void Main()
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletTemperature t = new BrickletTemperature(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		t.SetDebouncePeriod(10000);

		// Register threshold reached callback to function TemperatureReachedCB
		t.TemperatureReached += TemperatureReachedCB;

		// Configure threshold for "greater than 30 °C" (unit is °C/100)
		t.SetTemperatureCallbackThreshold('>', 30*100, 0);

		System.Console.WriteLine("Press enter to exit");
		System.Console.ReadLine();
		ipcon.Disconnect();
	}
}
