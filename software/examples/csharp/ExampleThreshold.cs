using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "ABC"; // Change to your UID

	// Callback for temperature greater than 30 °C
	static void ReachedCB(BrickletTemperature sender, short temperature)
	{
		System.Console.WriteLine("We have: " + temperature/100.0 + " °C");
		System.Console.WriteLine("It is too hot, we need air conditioning!");
	}

	static void Main() 
	{
		IPConnection ipcon = new IPConnection(); // Create IP connection
		BrickletTemperature temp = new BrickletTemperature(UID, ipcon); // Create device object

		ipcon.Connect(HOST, PORT); // Connect to brickd
		// Don't use device before ipcon is connected

		// Get threshold callbacks with a debounce time of 1 seconds (1000ms)
		temp.SetDebouncePeriod(1000);

		// Register threshold reached callback to function ReachedCB
		temp.TemperatureReached += ReachedCB;

		// Configure threshold for "greater than 30 °C" (unit is °C/100)
		temp.SetTemperatureCallbackThreshold('>', 30*100, 0);

		System.Console.WriteLine("Press key to exit");
		System.Console.ReadKey();
	}
}
