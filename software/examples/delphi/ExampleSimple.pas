program ExampleSimple;

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
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = '8o9';

var
  e: TExample;

procedure TExample.Execute;
var temperature: word;
begin
  { Create IP connection to brickd }
  ipcon := TIPConnection.Create(HOST, PORT);

  { Create device object }
  t := TBrickletTemperature.Create(UID);

  { Add device to IP connection }
  ipcon.AddDevice(t);
  { Don't use device before it is added to a connection }

  { Get current temperature (unit is °C/100) }
  temperature := t.GetTemperature;
  WriteLn(Format('Temperature: %f °C', [temperature/100.0]));

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy;
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
