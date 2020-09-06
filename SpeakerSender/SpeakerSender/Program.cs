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
		static object LockObj = new object();
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
			string ComPort = "COM9";

			Console.Write("Opening {0} ... ", ComPort);
			Com = new SerialPort(ComPort, 110000);
			Com.NewLine = "\n";
			Com.Open();
			Console.WriteLine("OK");

			Console.WriteLine("Baud rate {0}", Com.BaudRate);

			Random Rnd = new Random();
			string[] MusicFiles = Directory.GetFiles("music", "*.raw");
			string MusicFile = MusicFiles[Rnd.Next(0, MusicFiles.Length)];
			Console.WriteLine("Playing {0}", MusicFile);

			Music.Data = File.ReadAllBytes(MusicFile);

			Thread T = new Thread(SenderThread);
			T.IsBackground = true;
			T.Start();

			Thread RT = new Thread(ReceiverThread);
			RT.IsBackground = true;
			RT.Start();

			while (true) {
			}
		}

		static void ReceiverThread() {
			bool BinaryMode = false;

			while (true) {
				if (BinaryMode) {
					byte Low = (byte)Com.ReadByte();
					byte High = (byte)Com.ReadByte();
					ushort Num = (ushort)(((ushort)High << 8) | (ushort)Low);

					lock (LockObj) {
						ReqCount = Num;

						Console.CursorLeft = 0;
						Console.Write("Requested {0} sample bytes ... {1}", ReqCount, DateTime.Now.TimeOfDay);
					}
				} else {
					string Line = Com.ReadLine();
					bool Print = true;

					if (Line.EndsWith("DISCARD_LINE"))
						Print = false;

					switch (Line) {
						case "BINARY":
							BinaryMode = true;
							Print = false;
							Console.WriteLine("Switching to binary mode\n\n\n");
							break;
					}

					if (Print)
						Console.WriteLine(Line);
				}
			}
		}

		static void SenderThread() {
			Stopwatch SWatch = Stopwatch.StartNew();
			int Offset = 0;

			while (true) {
				if (ReqCount <= 0)
					continue;

				lock (LockObj) {
					byte[] ReqData = new byte[ReqCount];

					for (int i = 0; i < ReqCount; i++) {
						ReqData[i] = Music.Data[Offset++];

						if (Offset >= Music.Data.Length)
							Offset = 0;
					}

					ReqCount = 0;
					Send(ReqData);
				}
			}
		}
	}
}
