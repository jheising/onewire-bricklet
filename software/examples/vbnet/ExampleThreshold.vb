Imports Tinkerforge

Module ExampleThreshold
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID

    ' Callback function for temperature greater than 30 °C (parameter has unit °C/100)
    Sub TemperatureReachedCB(ByVal sender As BrickletTemperature, ByVal temperature As Short)
        System.Console.WriteLine("Temperature: " + (temperature/100.0).ToString() + " °C")
        System.Console.WriteLine("It is too hot, we need air conditioning!")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim t As New BrickletTemperature(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        t.SetDebouncePeriod(10000)

        ' Register threshold reached callback to function TemperatureReachedCB
        AddHandler t.TemperatureReached, AddressOf TemperatureReachedCB

        ' Configure threshold for "greater than 30 °C" (unit is °C/100)
        t.SetTemperatureCallbackThreshold(">"C, 30*100, 0)

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
