using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "ABC"; // Change to your UID

	static void Main() 
	{
		IPConnection ipcon = new IPConnection(HOST, PORT); // Create connection to brickd
		BrickletTemperature temp = new BrickletTemperature(UID); // Create device object
		ipcon.AddDevice(temp); // Add device to IP connection
		// Don't use device before it is added to a connection

		// Get current temperature (unit is °C/100)
		short temperature;
		temp.GetTemperature(out temperature);

		System.Console.WriteLine("Temperature: " + temperature/100.0 + " °C");

		System.Console.WriteLine("Press ctrl+c to exit");
		ipcon.JoinThread();
	}
}
