/*************************************************************
 * This file was automatically generated on 2011-09-20.      *
 *                                                           *
 * If you have a bugfix for this file and want to commit it, *
 * please fix the bug in the generator. You can find a link  *
 * to the generator git on tinkerforge.com                   *
 *************************************************************/

namespace Tinkerforge
{
	public class BrickletTemperature : Device 
	{
		private static byte TYPE_GET_TEMPERATURE = 1;
		private static byte TYPE_SET_TEMPERATURE_CALLBACK_PERIOD = 2;
		private static byte TYPE_GET_TEMPERATURE_CALLBACK_PERIOD = 3;
		private static byte TYPE_SET_TEMPERATURE_CALLBACK_THRESHOLD = 4;
		private static byte TYPE_GET_TEMPERATURE_CALLBACK_THRESHOLD = 5;
		private static byte TYPE_SET_DEBOUNCE_PERIOD = 6;
		private static byte TYPE_GET_DEBOUNCE_PERIOD = 7;
		private static byte TYPE_TEMPERATURE = 8;
		private static byte TYPE_TEMPERATURE_REACHED = 9;

		public delegate void Temperature(short temperature);
		public delegate void TemperatureReached(short temperature);

		public BrickletTemperature(string uid) : base(uid) 
		{
			messageCallbacks[TYPE_TEMPERATURE] = new MessageCallback(CallbackTemperature);
			messageCallbacks[TYPE_TEMPERATURE_REACHED] = new MessageCallback(CallbackTemperatureReached);
		}

		public void GetTemperature(out short temperature)
		{
			byte[] data = new byte[4];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_GET_TEMPERATURE, 1, data);
			LEConverter.To((ushort)4, 2, data);

			ipcon.Write(this, data, TYPE_GET_TEMPERATURE, true);

			byte[] answer;
			if(!answerQueue.TryDequeue(out answer, IPConnection.TIMEOUT_ANSWER))
			{
				throw new TimeoutException("Did not receive answer for GetTemperature in time");
			}

			temperature = LEConverter.ShortFrom(4, answer);

			writeEvent.Set();
		}

		public void SetTemperatureCallbackPeriod(uint period)
		{
			byte[] data = new byte[8];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_SET_TEMPERATURE_CALLBACK_PERIOD, 1, data);
			LEConverter.To((ushort)8, 2, data);
			LEConverter.To(period, 4, data);

			ipcon.Write(this, data, TYPE_SET_TEMPERATURE_CALLBACK_PERIOD, false);
		}

		public void GetTemperatureCallbackPeriod(out uint period)
		{
			byte[] data = new byte[4];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_GET_TEMPERATURE_CALLBACK_PERIOD, 1, data);
			LEConverter.To((ushort)4, 2, data);

			ipcon.Write(this, data, TYPE_GET_TEMPERATURE_CALLBACK_PERIOD, true);

			byte[] answer;
			if(!answerQueue.TryDequeue(out answer, IPConnection.TIMEOUT_ANSWER))
			{
				throw new TimeoutException("Did not receive answer for GetTemperatureCallbackPeriod in time");
			}

			period = LEConverter.UIntFrom(4, answer);

			writeEvent.Set();
		}

		public void SetTemperatureCallbackThreshold(char option, short min, short max)
		{
			byte[] data = new byte[9];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_SET_TEMPERATURE_CALLBACK_THRESHOLD, 1, data);
			LEConverter.To((ushort)9, 2, data);
			LEConverter.To(option, 4, data);
			LEConverter.To(min, 5, data);
			LEConverter.To(max, 7, data);

			ipcon.Write(this, data, TYPE_SET_TEMPERATURE_CALLBACK_THRESHOLD, false);
		}

		public void GetTemperatureCallbackThreshold(out char option, out short min, out short max)
		{
			byte[] data = new byte[4];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_GET_TEMPERATURE_CALLBACK_THRESHOLD, 1, data);
			LEConverter.To((ushort)4, 2, data);

			ipcon.Write(this, data, TYPE_GET_TEMPERATURE_CALLBACK_THRESHOLD, true);

			byte[] answer;
			if(!answerQueue.TryDequeue(out answer, IPConnection.TIMEOUT_ANSWER))
			{
				throw new TimeoutException("Did not receive answer for GetTemperatureCallbackThreshold in time");
			}

			option = LEConverter.CharFrom(4, answer);
			min = LEConverter.ShortFrom(5, answer);
			max = LEConverter.ShortFrom(7, answer);

			writeEvent.Set();
		}

		public void SetDebouncePeriod(uint debounce)
		{
			byte[] data = new byte[8];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_SET_DEBOUNCE_PERIOD, 1, data);
			LEConverter.To((ushort)8, 2, data);
			LEConverter.To(debounce, 4, data);

			ipcon.Write(this, data, TYPE_SET_DEBOUNCE_PERIOD, false);
		}

		public void GetDebouncePeriod(out uint debounce)
		{
			byte[] data = new byte[4];
			LEConverter.To(stackID, 0, data);
			LEConverter.To(TYPE_GET_DEBOUNCE_PERIOD, 1, data);
			LEConverter.To((ushort)4, 2, data);

			ipcon.Write(this, data, TYPE_GET_DEBOUNCE_PERIOD, true);

			byte[] answer;
			if(!answerQueue.TryDequeue(out answer, IPConnection.TIMEOUT_ANSWER))
			{
				throw new TimeoutException("Did not receive answer for GetDebouncePeriod in time");
			}

			debounce = LEConverter.UIntFrom(4, answer);

			writeEvent.Set();
		}

		public int CallbackTemperature(byte[] data)
		{
			short temperature = LEConverter.ShortFrom(4, data);

			((Temperature)callbacks[TYPE_TEMPERATURE])(temperature);
			return 6;
		}

		public int CallbackTemperatureReached(byte[] data)
		{
			short temperature = LEConverter.ShortFrom(4, data);

			((TemperatureReached)callbacks[TYPE_TEMPERATURE_REACHED])(temperature);
			return 6;
		}

		public void RegisterCallback(System.Delegate d)
		{
			if(d.GetType() == typeof(Temperature))
			{
				callbacks[TYPE_TEMPERATURE] = d;
			}
			else if(d.GetType() == typeof(TemperatureReached))
			{
				callbacks[TYPE_TEMPERATURE_REACHED] = d;
			}
		}
	}
}
