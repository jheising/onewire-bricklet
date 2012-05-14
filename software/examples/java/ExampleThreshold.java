import com.tinkerforge.BrickletTemperature;
import com.tinkerforge.IPConnection;

public class ExampleThreshold {
	private static final String host = "localhost";
	private static final int port = 4223;
	private static final String UID = "ABC"; // Change to your UID
	
	// Note: To make the example code cleaner we do not handle exceptions. Exceptions you
	//       might normally want to catch are described in the commnents below
	public static void main(String args[]) throws Exception {
		// Create connection to brickd
		IPConnection ipcon = new IPConnection(host, port); // Can throw IOException
		BrickletTemperature temp = new BrickletTemperature(UID); // Create device object

		// Add device to IP connection
		ipcon.addDevice(temp); // Can throw IPConnection.TimeoutException
		// Don't use device before it is added to a connection

		// Get threshold callbacks with a debounce time of 10 seconds (10000ms)
		temp.setDebouncePeriod(10000);

		// Configure threshold for "greater than 30 °C" (unit is °C/100)
		temp.setTemperatureCallbackThreshold('>', (short)(30*100), (short)0);

		// Add and implement temperature reached listener 
		// (called if temperature is greater than 30 °C)
		temp.addListener(new BrickletTemperature.TemperatureReachedListener() {
			public void temperatureReached(short temperature) {
				System.out.println("We have " + temperature/100.0 + " °C.");
				System.out.println("It is too hot, we need air conditioning!");
			}
		});

		System.out.println("Press ctrl+c to exit");
		ipcon.joinThread();
	}
}
