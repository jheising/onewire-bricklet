program ExampleThreshold;

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
    procedure ReachedCB(const temperature: smallint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = '8o9';

var
  e: TExample;

{ Callback for temperature greater than 30 °C }
procedure TExample.ReachedCB(const temperature: smallint);
begin
  WriteLn(Format('We have %f °C.', [temperature/100.0]));
  WriteLn('It is too hot, we need air conditioning!');
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

  { Get threshold callbacks with a debounce time of 10 seconds (10000ms) }
  t.SetDebouncePeriod(10000);

  { Register threshold reached callback to procedure ReachedCB }
  t.OnTemperatureReached := {$ifdef FPC}@{$endif}ReachedCB;

  { Configure threshold for "greater than 30 °C" (unit is °C/100) }
  t.SetTemperatureCallbackThreshold('>', 30*100, 0);

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy;
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
