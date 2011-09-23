using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "ABC"; // Change to your UID

	// Callback for temperature greater than 30 °C
	static void ReachedCB(short temperature)
	{
		System.Console.WriteLine("We have: " + temperature/100.0 + " °C");
		System.Console.WriteLine("It is too hot, we need air conditioning!");
	}

	static void Main() 
	{
		IPConnection ipcon = new IPConnection(HOST, PORT); // Create connection to brickd
		BrickletTemperature temp = new BrickletTemperature(UID); // Create device object
		ipcon.AddDevice(temp); // Add device to ip connection
		// Don't use device before it is added to a connection


		// Get threshold callbacks with a debounce time of 1 seconds (1000ms)
		temp.SetDebouncePeriod(1000);

		// Register threshold reached callback to function ReachedCB
		temp.RegisterCallback(new BrickletTemperature.TemperatureReached(ReachedCB));

		// Configure threshold for "greater than 30 °C" (unit is °C/100)
		temp.SetTemperatureCallbackThreshold('>', 30*100, 0);

		System.Console.WriteLine("Press ctrl+c to exit");
		ipcon.JoinThread();
    }
}
