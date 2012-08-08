program ExampleCallback;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletTemperature;

type
  TExample = class
  private
    ipcon: TIPConnection;
    t: TBrickletTemperature;
  public
    procedure TemperatureCB(const temperature: smallint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = '8o9'; { Change to your UID }

var
  e: TExample;

{ Callback function for temperature callback (parameter has unit °C/100) }
procedure TExample.TemperatureCB(const temperature: smallint);
begin
  WriteLn(Format('Temperature: %f °C', [temperature/100.0]));
end;

procedure TExample.Execute;
begin
  { Create IP connection to brickd }
  ipcon := TIPConnection.Create(HOST, PORT);

  { Create device object }
  t := TBrickletTemperature.Create(UID);

  { Add device to IP connection }
  ipcon.AddDevice(t);
  { Don't use device before it is added to a connection }

  { Set Period for temperature callback to 1s (1000ms)
    Note: The temperature callback is only called every second if the
          temperature has changed since the last call! }
  t.SetTemperatureCallbackPeriod(1000);

  { Register temperature callback to procedure TemperatureCB }
  t.OnTemperature := {$ifdef FPC}@{$endif}TemperatureCB;

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy;
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
