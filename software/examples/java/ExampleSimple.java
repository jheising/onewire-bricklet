import com.tinkerforge.BrickletTemperature;
import com.tinkerforge.IPConnection;

public class ExampleSimple {
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

		// Get current temperature (unit is °C/100)
		short temperature = temp.getTemperature(); // Can throw IPConnection.TimeoutException

		System.out.println("Temperature: " + temperature/100.0 + " °C");

		System.console().readLine("Press key to exit\n");
		ipcon.destroy();
	}
}
