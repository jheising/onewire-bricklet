Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Temperature Bricklet

    ' Callback subroutine for temperature callback (parameter has unit °C/100)
    Sub TemperatureCB(ByVal sender As BrickletTemperature, ByVal temperature As Short)
        Console.WriteLine("Temperature: " + (temperature/100.0).ToString() + " °C")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim t As New BrickletTemperature(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register temperature callback to subroutine TemperatureCB
        AddHandler t.Temperature, AddressOf TemperatureCB

        ' Set period for temperature callback to 1s (1000ms)
        ' Note: The temperature callback is only called every second
        '       if the temperature has changed since the last call!
        t.SetTemperatureCallbackPeriod(1000)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
