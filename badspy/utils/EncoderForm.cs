using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace utils
{
    public partial class EncoderForm : Form
    {
        public EncoderForm()
        {
            InitializeComponent();
        }

        private void btnConvert_Click(object sender, EventArgs e)
        {
            string st = txtInput.Text;
            StringBuilder builder = new StringBuilder();
            int min = 255;
            int max = 0;
            for (int i = 0; i < st.Length; i++)
            {
                char ch = st[i];
                int v = (int)ch;
                v += i % 2 == 0 ? 12 : 25;
                if (v < min)
                    min = v;
                if (v > max)
                    max = v;
                builder.Append(v);
                builder.Append(", ");
            }
            Text = "Min(" + min + ") Max(" + max + ")";
            txtOutput.Text = builder.ToString();
        }

        private void btnReConvert_Click(object sender, EventArgs e)
        {
            string st = txtOutput.Text;
            string[] parts = st.Split(',');
            StringBuilder builder = new StringBuilder();
            for (int i = 0; i < parts.Length; i++)
            {
                int v;
                if (int.TryParse(parts[i], out v))
                {
                    char ch = (char)(v);
                    builder.Append(ch);
                }
            }
            txtInput.Text = builder.ToString();
        }
    }
}
