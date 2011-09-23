using Tinkerforge;

class Example
{
	private static string HOST = "localhost";
	private static int PORT = 4223;
	private static string UID = "ABC"; // Change to your UID

	// Callback function for temperature callback (parameter has unit °C/100)
	static void TemperatureCB(short temperature)
	{
		System.Console.WriteLine("Temperature: " + temperature/100.0 + " °C");
	}

	static void Main() 
	{
		IPConnection ipcon = new IPConnection(HOST, PORT); // Create connection to brickd
		BrickletTemperature temp = new BrickletTemperature(UID); // Create device object
		ipcon.AddDevice(temp); // Add device to ip connection
		// Don't use device before it is added to a connection


		// Set Period for temperature callback to 1s (1000ms)
		// Note: The temperature callback is only called every second if the 
		//       temperature has changed since the last call!
		temp.SetTemperatureCallbackPeriod(1000);

		// Register temperature callback to function TemperatureCB
		temp.RegisterCallback(new BrickletTemperature.Temperature(TemperatureCB));

		System.Console.WriteLine("Press ctrl+c to exit");
		ipcon.JoinThread();
    }
}
