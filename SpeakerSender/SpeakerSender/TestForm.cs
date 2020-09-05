using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace SpeakerSender {
	public partial class TestForm : Form {
		public TestForm() {
			InitializeComponent();
		}

		private void TrackBar1_Scroll(object sender, EventArgs e) {
			label1.Text = trackBar1.Value.ToString();

			//Program.Send((byte)trackBar1.Value);
		}
	}
}
