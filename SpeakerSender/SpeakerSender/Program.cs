using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.IO.Ports;
using System.Threading;
using System.Diagnostics;
using System.Net.Http.Headers;

namespace SpeakerSender {
	unsafe static class Program {
		static SerialPort Com;
		static int ReqCount = 0;

		public static void Send(byte[] Tone) {
			Com.Write(Tone, 0, Tone.Length);
		}

		public static void Send(string Msg) {
			byte[] MsgBytes = Encoding.UTF8.GetBytes(Msg + "\n");
			Com.Write(MsgBytes, 0, MsgBytes.Length);
		}

		[STAThread]
		static void Main(string[] args) {
			string ComPort = File.ReadAllText("com_port.cfg").Trim();

			Console.Write("Opening {0} ... ", ComPort);
			Com = new SerialPort(ComPort, 200000/*, Parity.Odd, 8, StopBits.One*/);
			Com.NewLine = "\n";
			Com.Open();
			Console.WriteLine("OK");

			Console.WriteLine("Baud rate {0}", Com.BaudRate);

			Music.Data = File.ReadAllBytes("mario.raw");

			Thread T = new Thread(SenderThread);
			T.IsBackground = true;
			T.Start();

			Thread RT = new Thread(ReceiverThread);
			RT.IsBackground = true;
			RT.Start();

			//System.Windows.Forms.Application.Run(new TestForm());
			while (true) {
			}
		}

		static void ReceiverThread() {
			while (true) {
				byte Low = (byte)Com.ReadByte();
				byte High = (byte)Com.ReadByte();
				ushort Num = (ushort)(((ushort)High << 8) | (ushort)Low);
				ReqCount = Num;

				/*if (Line == "READY")
					Send("BINARY");*/
			}
		}

		static void SenderThread() {
			Stopwatch SWatch = Stopwatch.StartNew();
			int Offset = 0;

			while (true) {
				if (ReqCount <= 0)
					continue;

				//ReqCount = 1024;

				byte[] ReqData = new byte[ReqCount];

				for (int i = 0; i < ReqCount; i++) {
					ReqData[i] = Music.Data[Offset++];

					if (Offset >= Music.Data.Length)
						Offset = 0;
				}


				ReqCount = 0;
				Send(ReqData);
				//Thread.Sleep(10);
			}
		}

		static void ReceiverThread2() {
			//Stopwatch SWatch = Stopwatch.StartNew();

			while (true) {
				char Chr = (char)(byte)Com.ReadByte();
				Console.Write(Chr);

				/*Console.WriteLine("ms {0}", SWatch.Elapsed.TotalMilliseconds);
				SWatch.Restart();*/

				//string Line = Com.ReadLine();
				//Console.WriteLine(Line);

				/*if (Line == "READY")
					Send("BINARY");*/
			}
		}
	}
}
