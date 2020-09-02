using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.IO.Ports;
using System.Threading;
using System.Diagnostics;

namespace SpeakerSender {
	unsafe static class Program {
		static SerialPort Com;
		static byte[] Buffer = new byte[] { 0, 0 };

		static byte[] Music;

		public static void Send(byte Tone) {
			Buffer[0] = Tone;
			Com.Write(Buffer, 0, Buffer.Length);
		}

		[STAThread]
		static void Main(string[] args) {
			Console.Write("Opening COM port ... ");
			Com = new SerialPort("COM11", 9600, Parity.Odd, 8, StopBits.One);
			Com.NewLine = "\n";
			Com.Open();
			Console.WriteLine("OK");

			Music = File.ReadAllBytes("mario.raw");

			/*Thread T = new Thread(SenderThread);
			T.IsBackground = true;
			T.Start();*/

			System.Windows.Forms.Application.Run(new TestForm());

			while (true) {
				string Line = Com.ReadLine();
				Console.WriteLine(Line);
			}
		}

		static void SenderThread() {
			Stopwatch SWatch = Stopwatch.StartNew();
			int Offset = 0;

			while (true) {
				byte Frq = Music[Offset++];
				if (Offset >= Music.Length)
					Offset = 0;

				Send(Frq);

				Thread.Sleep(TimeSpan.FromSeconds(1.0f / 44100));
			}
		}
	}
}
